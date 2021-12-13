import { readInputAsLines } from "../../utils/readInput";

const lines = readInputAsLines();

interface IPosition {
  x: number;
  y: number;
}

type Axis = "x" | "y";

interface IFold {
  axis: Axis;
  value: number;
}

type Map = { [key: string]: "#" };

const map: Map = {};
const foldInstructions: IFold[] = [];

let doneWithDots = false;
lines.forEach((line) => {
  // check if we should start reading folding instructions
  if (line.length === 0 && !doneWithDots) {
    doneWithDots = true;
    return;
  }
  if (doneWithDots) {
    // remove the beginning of the strings
    const foldingInstructionRaw = line.slice("fold along ".length);
    const [axis, value] = foldingInstructionRaw.split("=");
    foldInstructions.push({
      axis: axis as IFold["axis"],
      value: Number(value),
    });
  } else {
    const [x, y] = line.split(",").map(Number);
    map[`${x};${y}`] = "#";
  }
});

function calculateOppositePoint(fold: IFold, point: IPosition): IPosition {
  if (fold.axis === "x") {
    return {
      y: point.y,
      x: point.x < fold.value ? point.x : fold.value - (point.x - fold.value),
    };
  } else {
    return {
      y: point.y < fold.value ? point.y : fold.value - (point.y - fold.value),
      x: point.x,
    };
  }
}

function drawMap(mapToDraw: Map) {
  const size = {
    width: 11,
    height: 15,
  };
  for (let y = 0; y < size.height; y++) {
    let line = "";
    for (let x = 0; x < size.width; x++) {
      if (mapToDraw[`${x};${y}`]) {
        line += "#";
      } else {
        line += ".";
      }
    }
    console.log(line);
  }
}

drawMap(map);

Object.keys(map).forEach((point) => {
  const [x, y] = point.split(";").map(Number);
  const newPosition = calculateOppositePoint(foldInstructions[0], { x, y });

  delete map[`${x};${y}`];
  map[`${newPosition.x};${newPosition.y}`] = "#";
});
drawMap(map);

console.log("Result:", Object.keys(map).length);

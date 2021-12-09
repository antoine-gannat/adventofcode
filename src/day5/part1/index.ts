import { readInputAsLines } from "../../utils/readInput";

interface IPosition {
  x: number;
  y: number;
}

interface IVentPosition {
  start: IPosition;
  end: IPosition;
}

type IOcean = { [key: string]: number };

function getVentPosition(rawLine: string): IVentPosition {
  const [rawStart, rawEnd] = rawLine.split(" -> ");

  const start = rawStart.split(",").map(Number);
  const end = rawEnd.split(",").map(Number);

  return {
    start: {
      x: start[0],
      y: start[1],
    },
    end: {
      x: end[0],
      y: end[1],
    },
  };
}

const data = readInputAsLines();

const ocean: IOcean = {};

data.forEach((line) => {
  const { start, end } = getVentPosition(line);

  // filter diagonals
  if (start.x !== end.x && start.y !== end.y) {
    return;
  }

  const xStart = start.x > end.x ? end.x : start.x;
  const xEnd = start.x > end.x ? start.x : end.x;
  for (let x = xStart; x <= xEnd; x++) {
    const yStart = start.y > end.y ? end.y : start.y;
    const yEnd = start.y > end.y ? start.y : end.y;
    for (let y = yStart; y <= yEnd; y++) {
      const key = `${x};${y}`;
      if (ocean[key] !== undefined) {
        ocean[key]++;
      } else {
        ocean[key] = 1;
      }
    }
  }
});

const total = Object.keys(ocean).reduce(
  (total, key) => (ocean[key] >= 2 ? total + 1 : total),
  0
);

console.log("Result: ", total);

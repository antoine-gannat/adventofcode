import { readInputAsLines } from "../../utils/readInput";

interface IPosition {
  x: number;
  y: number;
}

const data = readInputAsLines();

const size = {
  width: data[0].length,
  height: data.length,
};

// go through the data to find low points

function isLowPoint({ x, y }: IPosition): boolean {
  const currentValue = Number(data[y][x]);
  let sidesWithHigherValues = 0;
  const isSide =
    x === 0 || x === size.width - 1 || y === 0 || y === size.height - 1;
  const isCorner =
    (y === 0 || y === size.height - 1) && (x === 0 || x === size.width - 1);
  // check top
  if (y - 1 >= 0 && Number(data[y - 1][x]) > currentValue) {
    sidesWithHigherValues++;
  }
  // check bottom
  if (y + 1 < size.height && Number(data[y + 1][x]) > currentValue) {
    sidesWithHigherValues++;
  }
  // check left
  if (x - 1 >= 0 && Number(data[y][x - 1]) > currentValue) {
    sidesWithHigherValues++;
  }
  // check right
  if (x + 1 < size.width && Number(data[y][x + 1]) > currentValue) {
    sidesWithHigherValues++;
  }
  // only 3 neighbors if side
  if (isSide && !isCorner) {
    return sidesWithHigherValues >= 3;
  }
  // only 2 neighbors if corner
  if (isCorner) {
    return sidesWithHigherValues >= 2;
  }
  // else 4 neighbors
  return sidesWithHigherValues >= 4;
}

let totalRiskLevel = 0;
// from left to right
for (let y = 0; y < size.height; y++) {
  // from top to bottom
  for (let x = 0; x < size.width; x++) {
    if (isLowPoint({ x, y })) {
      totalRiskLevel += Number(data[y][x]) + 1;
    }
  }
}

console.log("Result:", totalRiskLevel);

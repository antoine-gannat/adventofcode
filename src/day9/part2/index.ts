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

const basinsSizes: number[] = [];

function calculateBasinSizeFromPoint(
  { x, y }: IPosition,
  visitedPoints: IPosition[] = []
): number {
  const currentPointHeight = Number(data[y][x]);
  visitedPoints.push({ x, y });

  function canGoToPoint({ x, y }: IPosition): boolean {
    if (x < 0 || y < 0 || x >= size.width || y >= size.height) {
      return false;
    }
    const hasHigherHeight =
      Number(data[y][x]) > currentPointHeight && Number(data[y][x]) < 9;
    return (
      hasHigherHeight &&
      // hasn't been visited
      !visitedPoints.find((pos) => pos.x === x && pos.y === y)
    );
  }

  let basinSize = 1;
  // check if we can go to the right
  if (canGoToPoint({ x: x + 1, y })) {
    basinSize += calculateBasinSizeFromPoint({ y, x: x + 1 }, visitedPoints);
  }
  // check to the left
  if (canGoToPoint({ x: x - 1, y })) {
    basinSize += calculateBasinSizeFromPoint({ y, x: x - 1 }, visitedPoints);
  }
  // check if we can go down
  if (canGoToPoint({ y: y + 1, x })) {
    basinSize += calculateBasinSizeFromPoint({ y: y + 1, x }, visitedPoints);
  }
  // check if we can go up
  if (canGoToPoint({ y: y - 1, x })) {
    basinSize += calculateBasinSizeFromPoint({ y: y - 1, x }, visitedPoints);
  }
  // recursively check relative positions
  return basinSize;
}

// from left to right
for (let y = 0; y < size.height; y++) {
  // from top to bottom
  for (let x = 0; x < size.width; x++) {
    // 9 cannot be a basin
    if (data[y][x] === "9") {
      continue;
    }
    const basinPositions: IPosition[] = [];
    const currentBasinSize = calculateBasinSizeFromPoint(
      { x, y },
      basinPositions
    );
    basinsSizes.push(currentBasinSize);
  }
}

// sort largest basins out of all basins
const largestBasins = basinsSizes
  .sort((a, b) => {
    if (a > b) {
      return -1;
    } else if (a < b) {
      return 1;
    }
    return 0;
  })
  .slice(0, 3);

let result = largestBasins[0] * largestBasins[1] * largestBasins[2];
console.log("Result:", result);

import { readInputAsLines } from "../../utils/readInput";

interface IPosition {
  x: number;
  y: number;
}

const data = readInputAsLines();
const octopuses = data.map((octLine) => octLine.split("").map(Number));

const size = {
  width: octopuses[0].length,
  height: octopuses.length,
};

const nbSteps = 100;

function increaseAllBy1() {
  for (let y = 0; y < size.height; y++) {
    for (let x = 0; x < size.width; x++) {
      octopuses[y][x]++;
    }
  }
}

function increaseNeighborOctopus({ x, y }: IPosition): boolean {
  if (x < 0 || y < 0 || x >= size.width || y >= size.height) {
    return false;
  }
  if (octopuses[y][x] === 0) {
    return false;
  }
  octopuses[y][x]++;
  if (octopuses[y][x] > 9) {
    return true;
  }
  return false;
}

let totalFlashes = 0;

function checkForFlahes() {
  let hasOctopusReady = true;
  while (hasOctopusReady) {
    hasOctopusReady = false;
    for (let y = 0; y < size.height; y++) {
      for (let x = 0; x < size.width; x++) {
        if (octopuses[y][x] > 9) {
          totalFlashes++;
          octopuses[y][x] = 0;
          // top neighbor
          increaseNeighborOctopus({ y: y - 1, x }) && (hasOctopusReady = true);
          // bottom neighbor
          increaseNeighborOctopus({ y: y + 1, x }) && (hasOctopusReady = true);
          // left neighbor
          increaseNeighborOctopus({ y, x: x - 1 }) && (hasOctopusReady = true);
          // bottom neighbor
          increaseNeighborOctopus({ y, x: x + 1 }) && (hasOctopusReady = true);
          // top Left diag
          increaseNeighborOctopus({ y: y - 1, x: x - 1 }) &&
            (hasOctopusReady = true);
          // topRight diag
          increaseNeighborOctopus({ y: y - 1, x: x + 1 }) &&
            (hasOctopusReady = true);
          // bottomLeft diag
          increaseNeighborOctopus({ y: y + 1, x: x - 1 }) &&
            (hasOctopusReady = true);
          // bottomRight diag
          increaseNeighborOctopus({ y: y + 1, x: x + 1 }) &&
            (hasOctopusReady = true);
        }
      }
    }
  }
}

for (let step = 0; step < nbSteps; step++) {
  // start the process by increasing by 1 all octopuses
  increaseAllBy1();
  checkForFlahes();
}

console.log("Result:", totalFlashes);

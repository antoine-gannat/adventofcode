import { readInput } from "../../utils/readInput";

const crabs = readInput().split(",").map(Number);

// just brute force the response
const min = Math.min(...crabs);
const max = Math.max(...crabs);

const fuelUsedPerPos: { [pos: string]: number } = {};

for (let pos = min; pos <= max; pos++) {
  fuelUsedPerPos[pos] = 0;
  crabs.forEach((crabPos) => {
    const n = Math.abs(crabPos - pos);
    fuelUsedPerPos[pos] += (n * (n + 1)) / 2;
  });
}

const minFuel = Math.min(
  ...Object.keys(fuelUsedPerPos).map((k) => fuelUsedPerPos[k])
);

console.log({ minFuel });

// 5
// 1+2+3+4+5

import { readInput } from "../../utils/readInput";

const crabs = readInput().split(",").map(Number);

// just brute force the response
const min = Math.min(...crabs);
const max = Math.max(...crabs);

const fuelUsedPerPos: { [pos: string]: number } = {};

for (let pos = min; pos <= max; pos++) {
  fuelUsedPerPos[pos] = 0;
  crabs.forEach((crabPos) => (fuelUsedPerPos[pos] += Math.abs(crabPos - pos)));
}

const minFuel = Math.min(
  ...Object.keys(fuelUsedPerPos).map((k) => fuelUsedPerPos[k])
);

console.log({ minFuel });

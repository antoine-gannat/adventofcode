import { readInputAsLines } from "../../utils/readInput";

const data = readInputAsLines();

const count: { [segment: string]: number } = {
  seg1: 0,
  seg4: 0,
  seg7: 0,
  seg8: 0,
};

for (const line of data) {
  const outputValues = line
    .split("|")[1]
    .split(" ")
    .map((v) => v.trim());
  count.seg1 += outputValues.filter((v) => v.length === 2).length;
  count.seg4 += outputValues.filter((v) => v.length === 4).length;
  count.seg7 += outputValues.filter((v) => v.length === 3).length;
  count.seg8 += outputValues.filter((v) => v.length === 7).length;
}

const total = Object.keys(count)
  .map((k) => count[k])
  .reduce((total, v) => total + v, 0);

console.log({ count }, total);

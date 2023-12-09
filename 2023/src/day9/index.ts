import { read } from "../utils";

const fileContent = read(9)
  .split("\n")
  .map((l) => l.match(/-?\d+/g).map(Number));

let total = 0;

fileContent.forEach((input) => {
  const numbers: number[][] = [input];

  // while the last entry is not only zeroes
  while (!numbers[numbers.length - 1].every((n) => n === 0)) {
    const last = numbers[numbers.length - 1];
    const next = last.slice(0, -1).map((n, index) => last[index + 1] - n);
    numbers.push(next);
  }

  numbers[numbers.length - 1].push(0);

  for (let i = numbers.length - 2; i >= 0; i--) {
    const currentRow = numbers[i];
    const nextRow = numbers[i + 1];
    const newVal =
      // concat the first number of current row + the first number of next row
      currentRow[0] - nextRow[0];
    currentRow.unshift(newVal);
  }

  total += numbers[0][0];
});

console.log(total);

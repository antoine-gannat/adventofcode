import { read } from "../utils";

const fileContent = read(3).split("\n");

function isNumber(char: string): boolean {
  return char >= "0" && char <= "9";
}

function isGear(char: string): boolean {
  return char === "*";
}

function extractFullNumber(
  x: number,
  y: number,
  visitedNumbers: { x: number; y: number }[]
): number {
  // go to the beginning of the number
  while (x > 0 && isNumber(fileContent[y][x - 1])) {
    x--;
  }
  if (visitedNumbers.find((n) => n.x === x && n.y === y)) {
    return -1;
  }
  visitedNumbers.push({ x, y });
  // extract the number
  let number = "";
  while (x < fileContent[y].length && isNumber(fileContent[y][x])) {
    number += fileContent[y][x];
    x++;
  }
  return parseInt(number);
}

function findNearbyNumbers(x: number, y: number): number[] {
  const numbers: number[] = [];
  const visitedNumbers: { x: number; y: number }[] = [];
  // check for left
  if (x > 0 && isNumber(fileContent[y][x - 1])) {
    numbers.push(extractFullNumber(x - 1, y, visitedNumbers));
  }
  // check for right
  if (x < fileContent[y].length - 1 && isNumber(fileContent[y][x + 1])) {
    numbers.push(extractFullNumber(x + 1, y, visitedNumbers));
  }
  // check for top
  if (y > 0 && isNumber(fileContent[y - 1][x])) {
    numbers.push(extractFullNumber(x, y - 1, visitedNumbers));
  }
  // check for bottom
  if (y < fileContent.length - 1 && isNumber(fileContent[y + 1][x])) {
    numbers.push(extractFullNumber(x, y + 1, visitedNumbers));
  }
  // check for top left
  if (x > 0 && y > 0 && isNumber(fileContent[y - 1][x - 1])) {
    numbers.push(extractFullNumber(x - 1, y - 1, visitedNumbers));
  }
  // check for top right
  if (
    x < fileContent[y].length - 1 &&
    y > 0 &&
    isNumber(fileContent[y - 1][x + 1])
  ) {
    numbers.push(extractFullNumber(x + 1, y - 1, visitedNumbers));
  }
  // check for bottom left
  if (
    x > 0 &&
    y < fileContent.length - 1 &&
    isNumber(fileContent[y + 1][x - 1])
  ) {
    numbers.push(extractFullNumber(x - 1, y + 1, visitedNumbers));
  }
  // check for bottom right
  if (
    x < fileContent[y].length - 1 &&
    y < fileContent.length - 1 &&
    isNumber(fileContent[y + 1][x + 1])
  ) {
    numbers.push(extractFullNumber(x + 1, y + 1, visitedNumbers));
  }

  return numbers.filter((n) => n !== -1);
}

let total = 0;

for (let y = 0; y < fileContent.length; y++) {
  for (let x = 0; x < fileContent[y].length; x++) {
    const char = fileContent[y][x];
    if (!isGear(char)) {
      continue;
    }
    // find numbers around the gear
    const numbers = findNearbyNumbers(x, y);
    if (numbers.length > 1) {
      total += numbers.reduce((a, b) => a * b, 1);
    }
  }
}

console.log(total);

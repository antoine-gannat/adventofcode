import { read } from "../utils";

const fileContent = read(3).split("\n");

function isNumber(char: string): boolean {
  return char >= "0" && char <= "9";
}

function isSymbol(char: string): boolean {
  return !isNumber(char) && char !== "." && char !== "\r";
}

function hasNearbySymbols(x: number, y: number): boolean {
  // check for left
  if (x > 0 && isSymbol(fileContent[y][x - 1])) {
    return true;
  }
  // check for right
  if (x < fileContent[y].length - 1 && isSymbol(fileContent[y][x + 1])) {
    return true;
  }
  // check for top
  if (y > 0 && isSymbol(fileContent[y - 1][x])) {
    return true;
  }
  // check for bottom
  if (y < fileContent.length - 1 && isSymbol(fileContent[y + 1][x])) {
    return true;
  }
  // check for top left
  if (x > 0 && y > 0 && isSymbol(fileContent[y - 1][x - 1])) {
    return true;
  }
  // check for top right
  if (
    x < fileContent[y].length - 1 &&
    y > 0 &&
    isSymbol(fileContent[y - 1][x + 1])
  ) {
    return true;
  }
  // check for bottom left
  if (
    x > 0 &&
    y < fileContent.length - 1 &&
    isSymbol(fileContent[y + 1][x - 1])
  ) {
    return true;
  }
  // check for bottom right
  if (
    x < fileContent[y].length - 1 &&
    y < fileContent.length - 1 &&
    isSymbol(fileContent[y + 1][x + 1])
  ) {
    return true;
  }

  return false;
}

let total = 0;

let currentNb = "";
let currentNbIsNearSymbol = false;

for (let y = 0; y < fileContent.length; y++) {
  for (let x = 0; x < fileContent[y].length; x++) {
    const char = fileContent[y][x];
    if (isNumber(char)) {
      currentNb += char;
      if (hasNearbySymbols(x, y)) {
        currentNbIsNearSymbol = true;
      }
    } else if (currentNb) {
      if (currentNbIsNearSymbol) {
        total += parseInt(currentNb);
        console.log(currentNb, "is near a symbol");
      } else {
        //console.log(currentNb, "is not near a symbol");
      }
      currentNb = "";
      currentNbIsNearSymbol = false;
    }
  }
}

console.log(total);

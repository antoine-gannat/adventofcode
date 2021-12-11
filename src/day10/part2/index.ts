import { readInputAsLines } from "../../utils/readInput";

type ClosingChars = ")" | "]" | "}" | ">";
type OpeningChars = "(" | "[" | "{" | "<";

interface ICharPair {
  opening: OpeningChars;
  closing: ClosingChars;
}

const charPairs: ICharPair[] = [
  {
    opening: "(",
    closing: ")",
  },
  {
    opening: "<",
    closing: ">",
  },
  {
    opening: "[",
    closing: "]",
  },
  {
    opening: "{",
    closing: "}",
  },
];

const data = readInputAsLines();

const pointsPerChar: { [key in ClosingChars]: number } = {
  ")": 1,
  "]": 2,
  "}": 3,
  ">": 4,
};

function getCharPair(char: string): ICharPair {
  return charPairs.find(
    ({ opening, closing }) => opening === char || closing === char
  );
}

function isOpeningChar(char: string): boolean {
  return char === "(" || char === "[" || char === "{" || char === "<";
}

function parseLine(line: string): number | false {
  let result = 0;
  const openedStack: OpeningChars[] = [];
  for (let i = 0; i < line.length; i++) {
    const pair = getCharPair(line[i]);
    if (isOpeningChar(line[i])) {
      openedStack.push(pair.opening);
    } else {
      const removedEl = openedStack.pop();
      if (removedEl !== pair.opening) {
        return false;
      }
    }
  }
  // now close the remaining opened pairs
  for (let i = openedStack.length - 1; i >= 0; i--) {
    const pair = getCharPair(openedStack[i]);
    result = result * 5 + pointsPerChar[pair.closing];
  }
  return result;
}

const lineResults = data
  .map(parseLine)
  .filter((l) => l !== false)
  .sort((a, b) => {
    if (a > b) {
      return -1;
    } else if (a < b) {
      return 1;
    }
    return 0;
  });

console.log("Result:", lineResults[Math.floor(lineResults.length / 2)]);

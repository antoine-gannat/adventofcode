import { readInputAsLines } from "../../utils/readInput";

interface IPairInsertionRule {
  pair: string;
  elementToInsert: string;
}

const data = readInputAsLines();

const nbSteps = 10;

let polymer = data[0];

const pairInsertionsRules: IPairInsertionRule[] = [];

function findPair(a: string, b: string): IPairInsertionRule {
  return pairInsertionsRules.find(({ pair }) => pair[0] === a && pair[1] === b);
}

for (let i = 2; i < data.length; i++) {
  const [pair, elementToInsert] = data[i].split(" -> ");
  pairInsertionsRules.push({ pair, elementToInsert });
}

for (let step = 0; step < nbSteps; step++) {
  let newPolymer = "";
  for (let i = 0; i < polymer.length; i++) {
    newPolymer += polymer[i];
    if (i + 1 >= polymer.length) {
      continue;
    }
    const pair = findPair(polymer[i], polymer[i + 1]);

    newPolymer += pair.elementToInsert;
  }
  polymer = newPolymer;
}

const finalCount: { [element: string]: number } = {};

for (let i = 0; i < polymer.length; i++) {
  const element = polymer[i];
  if (!finalCount[element]) {
    finalCount[element] = 1;
  } else {
    finalCount[element]++;
  }
}

const sortedCount = Object.keys(finalCount)
  .map((key) => finalCount[key])
  .sort((a, b) => {
    if (a > b) {
      return -1;
    } else if (a < b) {
      return 1;
    }
    return 0;
  });

console.log("Result:", sortedCount[0] - sortedCount[sortedCount.length - 1]);

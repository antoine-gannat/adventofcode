// day to reproduce: 7
// for new board: 9

import { readInput } from "../../utils/readInput";

let fishes = readInput().split(",").map(Number);

for (let day = 0; day < 80; day++) {
  let nbFishToBeAdded = 0;
  fishes = fishes.map((fish) => {
    if (fish === 0) {
      nbFishToBeAdded++;
      return 6;
    }
    return fish - 1;
  });
  // add new born fishes
  for (let i = 0; i < nbFishToBeAdded; i++) {
    fishes.push(8);
  }
}
console.log("Result:", fishes.length);

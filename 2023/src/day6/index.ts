import { read } from "../utils";

const fileContent = read(6).replace(/ /gm, "").split("\n");

const raceDurations = fileContent[0].match(/(\d+)/gm).map(Number);
const bestDistance = fileContent[1].match(/(\d+)/gm).map(Number);

let result = 1;

for (let raceNb = 0; raceNb < raceDurations.length; raceNb++) {
  let waysToWin = 0;

  for (let i = 1; i < raceDurations[raceNb] - 1; i++) {
    const distance = (raceDurations[raceNb] - i) * i;
    if (distance > bestDistance[raceNb]) {
      waysToWin++;
    }
  }
  result *= waysToWin;
}

console.log(result);

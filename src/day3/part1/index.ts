import { readInput } from "../../utils/readInput";

const data = readInput();

const binaryGrid = data.split("\r\n");

const gridSize = {
  width: binaryGrid[0].length,
  height: binaryGrid.length,
};

function getColumnMostCommon(column: number): "1" | "0" {
  let total0 = 0;
  let total1 = 0;

  for (let line = 0; line < gridSize.height; line++) {
    switch (binaryGrid[line][column]) {
      case "1":
        total1++;
        break;
      case "0":
        total0++;
        break;
    }
  }
  return total0 > total1 ? "0" : "1";
}

// calculate gamma rate

let gamma = "";

for (let col = 0; col < gridSize.width; col++) {
  gamma += getColumnMostCommon(col);
}

// calculate epsilon by getting the oposite of gamma
const epsilon = gamma
  .split("")
  .map((b) => (b === "1" ? "0" : "1"))
  .join("");

const gammaDec = parseInt(gamma, 2);
const epsilonDec = parseInt(epsilon, 2);

console.log("Power consumption: ", gammaDec * epsilonDec);

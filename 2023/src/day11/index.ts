import { read } from "../utils";

interface IPoint {
  x: number;
  y: number;
}

const fileContent = read(11).split("\n");

const colsWithNoGalaxies: number[] = [];
const rowsWithNoGalaxies: number[] = [];

const emptyMultiplier = 1000000;

const galaxyMap: string[] = [];

const galaxies: IPoint[] = [];

// Add new lines to the map where the is no galaxy
for (let y = 0; y < fileContent.length; y++) {
  galaxyMap.push(fileContent[y]);
  if (!fileContent[y].split("").some((col) => col === "#")) {
    rowsWithNoGalaxies.push(y);
  }
}

// Add new columns to the map where the is no galaxy
for (let x = 0; x < fileContent[0].length; x++) {
  let hasGalaxies = false;
  for (let y = 0; y < fileContent.length; y++) {
    if (fileContent[y][x] === "#") {
      hasGalaxies = true;
    }
  }
  if (!hasGalaxies) {
    colsWithNoGalaxies.push(x);
  }
}

// find the galaxies on the new map
for (let y = 0; y < galaxyMap.length; y++) {
  for (let x = 0; x < galaxyMap[y].length; x++) {
    if (galaxyMap[y][x] === "#") {
      // add the offset of empty rows and columns that precede this galaxy
      const nbEmptyRows = rowsWithNoGalaxies.filter((row) => row < y).length;
      const nbEmptyCols = colsWithNoGalaxies.filter((col) => col < x).length;
      galaxies.push({
        x: x + nbEmptyCols * (emptyMultiplier - 1),
        y: y + nbEmptyRows * (emptyMultiplier - 1),
      });
    }
  }
}

const visitedDistances: Record<string, boolean> = {};

let total = 0;

function distanceBetweenPoints(pt1: IPoint, pt2: IPoint): number {
  return Math.abs(pt1.x - pt2.x) + Math.abs(pt1.y - pt2.y);
}

// calculate the shortest path between galaxies
galaxies.forEach((galaxyFrom, indexFrom) => {
  galaxies.forEach((galaxyTarget, indexTarget) => {
    // skip the same galaxy and already visited distances
    if (
      indexFrom === indexTarget ||
      visitedDistances[`${indexFrom}-${indexTarget}`] ||
      visitedDistances[`${indexTarget}-${indexFrom}`]
    ) {
      return;
    }
    const shortestDistance = distanceBetweenPoints(galaxyFrom, galaxyTarget);
    visitedDistances[`${indexFrom}-${indexTarget}`] = true;
    total += shortestDistance;
  });
});

console.log(total);

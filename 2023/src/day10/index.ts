import { read } from "../utils";

interface IPipe {
  position: IPosition;
  north?: IPipe;
  south?: IPipe;
  east?: IPipe;
  west?: IPipe;
  distance?: number;
  char: string;
}

interface IPosition {
  x: number;
  y: number;
}

// pipes that can be found to the north of the current pipe
const validNorthPipes = ["|", "7", "F", "S"];
// pipes that can be found to the south of the current pipe
const validSouthPipes = ["|", "L", "J", "S"];
// pipes that can be found to the east of the current pipe
const validEastPipes = ["-", "7", "J", "S"];
// pipes that can be found to the west of the current pipe
const validWestPipes = ["-", "L", "F", "S"];

let fileContent = read(10).split("\n");

function findStart(): IPosition {
  const line = fileContent.findIndex((line) => line.includes("S"));
  const column = fileContent[line].indexOf("S");

  return { x: column, y: line };
}

function extractPipes(currentPipe: IPipe, pipes: IPipe[]): void {
  const { position, distance } = currentPipe;
  // check for a pipe north of the current position
  if (
    validNorthPipes.includes(fileContent[position.y - 1]?.[position.x]) &&
    !pipes.some(
      (pipe) =>
        pipe.position.x === position.x && pipe.position.y === position.y - 1
    )
  ) {
    currentPipe.north = {
      position: { x: position.x, y: position.y - 1 },
      distance: distance + 1,
      char: fileContent[position.y - 1][position.x],
    };
    pipes.push(currentPipe.north);
  }
  // check for a pipe south of the current position
  if (
    validSouthPipes.includes(fileContent[position.y + 1]?.[position.x]) &&
    !pipes.some(
      (pipe) =>
        pipe.position.x === position.x && pipe.position.y === position.y + 1
    )
  ) {
    currentPipe.south = {
      position: { x: position.x, y: position.y + 1 },
      distance: distance + 1,
      char: fileContent[position.y + 1][position.x],
    };
    pipes.push(currentPipe.south);
  }
  // check for a pipe east of the current position
  if (
    validEastPipes.includes(fileContent[position.y]?.[position.x + 1]) &&
    !pipes.some(
      (pipe) =>
        pipe.position.x === position.x + 1 && pipe.position.y === position.y
    )
  ) {
    currentPipe.east = {
      position: { x: position.x + 1, y: position.y },
      distance: distance + 1,
      char: fileContent[position.y][position.x + 1],
    };
    pipes.push(currentPipe.east);
  }
  // check for a pipe west of the current position
  if (
    validWestPipes.includes(fileContent[position.y]?.[position.x - 1]) &&
    !pipes.some(
      (pipe) =>
        pipe.position.x === position.x - 1 && pipe.position.y === position.y
    )
  ) {
    currentPipe.west = {
      position: { x: position.x - 1, y: position.y },
      distance: distance + 1,
      char: fileContent[position.y][position.x - 1],
    };
    pipes.push(currentPipe.west);
  }
}

function canReachOutside(start: IPosition): {
  visited: IPosition[];
  isEnclosed: boolean;
} {
  const visited: IPosition[] = [];
  const toVisit: IPosition[] = [start];

  while (toVisit.length > 0) {
    const current = toVisit.shift();
    if (!current) {
      continue;
    }
    if (
      current.x === 0 ||
      current.x === fileContent[0].length - 1 ||
      current.y === 0 ||
      current.y === fileContent.length - 1
    ) {
      return { visited, isEnclosed: false };
    }
    visited.push(current);
    // if there is no pipe north add it to the list of places to visit
    if (
      !pipes.some(
        (pipe) =>
          pipe.position.x === current.x && pipe.position.y === current.y - 1
      ) &&
      !visited.some((pos) => pos.x === current.x && pos.y === current.y - 1) &&
      !toVisit.find((pos) => pos.x === current.x && pos.y === current.y - 1)
    ) {
      toVisit.push({ x: current.x, y: current.y - 1 });
    }
    // if there is no pipe south add it to the list of places to visit
    if (
      !pipes.some(
        (pipe) =>
          pipe.position.x === current.x && pipe.position.y === current.y + 1
      ) &&
      !visited.some((pos) => pos.x === current.x && pos.y === current.y + 1) &&
      !toVisit.find((pos) => pos.x === current.x && pos.y === current.y + 1)
    ) {
      toVisit.push({ x: current.x, y: current.y + 1 });
    }
    // if there is no pipe east add it to the list of places to visit
    if (
      !pipes.some(
        (pipe) =>
          pipe.position.x === current.x + 1 && pipe.position.y === current.y
      ) &&
      !visited.some((pos) => pos.x === current.x + 1 && pos.y === current.y) &&
      !toVisit.find((pos) => pos.x === current.x + 1 && pos.y === current.y)
    ) {
      toVisit.push({ x: current.x + 1, y: current.y });
    }
    // if there is no pipe west add it to the list of places to visit
    if (
      !pipes.some(
        (pipe) =>
          pipe.position.x === current.x - 1 && pipe.position.y === current.y
      ) &&
      !visited.some((pos) => pos.x === current.x - 1 && pos.y === current.y) &&
      !toVisit.find((pos) => pos.x === current.x - 1 && pos.y === current.y)
    ) {
      toVisit.push({ x: current.x - 1, y: current.y });
    }
  }

  return { visited, isEnclosed: true };
}

// increase the size of the starting map to create paths between pipes
console.log("Extending map");
const newFileContent: string[][] = fileContent
  .map((line) => line.split("").join(" "))
  .map((line) => [
    line,
    Array.from({ length: line.length })
      .map(() => " ")
      .join(""),
  ])
  .flat()
  .map((line) => line.split(""));
console.log("Processing new empty cells");

for (let y = 0; y < newFileContent.length; y++) {
  for (let x = 0; x < newFileContent[y].length; x++) {
    if (newFileContent[y][x] !== " ") {
      continue;
    }
    const north = newFileContent[y - 1]?.[x];
    const south = newFileContent[y + 1]?.[x];
    const east = newFileContent[y]?.[x + 1];
    const west = newFileContent[y]?.[x - 1];
    // if we have a vertical pipe above and below we can create a path between them
    if (validNorthPipes.includes(north) && validSouthPipes.includes(south)) {
      newFileContent[y][x] = "|";
    } else if (
      // if we have a horizontal pipe to the left and right we can create a path between them
      validEastPipes.includes(east) &&
      validWestPipes.includes(west)
    ) {
      newFileContent[y][x] = "-";
    } else {
      newFileContent[y][x] = ".";
    }
  }
}

fileContent = newFileContent.map((line) => line.join(""));

const pipes: IPipe[] = [
  {
    position: findStart(),
    distance: 0,
    char: "S",
  },
];
console.log("Extracting pipes");

for (let i = 0; i < pipes.length; i++) {
  extractPipes(pipes[i], pipes);
}

const mapWithEnclosed: string[][] = Array.from({
  length: fileContent.length,
}).map(() => []);

console.log("Calculating enclosed area");
for (let y = 0; y < fileContent.length; y++) {
  for (let x = 0; x < fileContent[y].length; x++) {
    const pipe = pipes.find(
      (pipe) => pipe.position.x === x && pipe.position.y === y
    );
    if (mapWithEnclosed[y][x] === "O" || mapWithEnclosed[y][x] === "I") {
      continue;
    }
    if (!pipe) {
      const { isEnclosed, visited } = canReachOutside({ x, y });
      visited.forEach(
        ({ x, y }) => (mapWithEnclosed[y][x] = isEnclosed ? "I" : "O")
      );
    } else {
      mapWithEnclosed[y][x] = pipe.char;
    }
  }
}

console.log("Revert enlargement");

// now revert the enlargement of the map
// Remove every other line and column
const revertedMap: string[][] = [];
for (let y = 0; y < mapWithEnclosed.length; y += 2) {
  revertedMap.push([]);
  for (let x = 0; x < mapWithEnclosed[y].length; x += 2) {
    revertedMap[revertedMap.length - 1].push(mapWithEnclosed[y][x]);
  }
}

// calculate the total enclosed area
const totalEnclosed = revertedMap.reduce(
  (acc, line) => (acc += line.filter((char) => char === "I").length),
  0
);
console.log(totalEnclosed);

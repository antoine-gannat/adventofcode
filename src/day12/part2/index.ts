import { readInputAsLines } from "../../utils/readInput";

interface ICave {
  name: string;
  // store the name of connections
  connections: string[];
  size: "big" | "small";
}

const endCaveName = "end";
const startCaveName = "start";

const data = readInputAsLines().map((line) => line.split("-"));

const caves: ICave[] = [];

function findCave(caveName: string): ICave | undefined {
  return caves.find(({ name }) => name === caveName);
}

function addCave(caveName: string) {
  if (findCave(caveName)) {
    return;
  }
  caves.push({
    name: caveName,
    connections: [],
    // if name uses uppercase letters, it's a big cave
    size: caveName.charCodeAt(0) < 97 ? "big" : "small",
  });
}

function addConnection(receivingCave: ICave, caveToAdd: ICave) {
  // if that connection already exists, leave
  if (receivingCave.connections.includes(caveToAdd.name)) {
    return;
  }
  receivingCave.connections.push(caveToAdd.name);
}

function pathFinder(
  entryCave: ICave,
  visitedCaves: string[],
  pathsFound: string[][],
  visitedTwice: boolean
) {
  // if the exit is found, leave
  if (entryCave.name === endCaveName) {
    pathsFound.push([...visitedCaves, entryCave.name]);
    return;
  }

  // add the current cave to the visited caves
  visitedCaves.push(entryCave.name);

  // for each connection
  for (const connection of entryCave.connections) {
    const cave = findCave(connection);
    // don't visit twice a small cave
    if (cave.name !== endCaveName && cave.size === "small") {
      const hasBeenVisited = visitedCaves.includes(cave.name);
      // if the cave has already been visited and we already visited twice an other cave
      // leave
      if ((hasBeenVisited && visitedTwice) || cave.name == startCaveName) {
        continue;
      } else if (hasBeenVisited && !visitedTwice) {
        // recursivelly visit connections
        pathFinder(cave, [...visitedCaves], pathsFound, true);
        continue;
      }
    }
    // recursivelly visit connections
    pathFinder(cave, [...visitedCaves], pathsFound, visitedTwice);
  }
}

// start by parsing the input

// go through the data a first time to load all caves
data.forEach(([caveA, caveB]) => {
  // if the cave is not already present in the array add it
  addCave(caveA);
  addCave(caveB);
});

// go through the data a second time to load all connections
data.forEach(([caveAName, caveBName]) => {
  const caveA = findCave(caveAName);
  const caveB = findCave(caveBName);
  addConnection(caveA, caveB);
  addConnection(caveB, caveA);
});

let pathsFound: string[][] = [];
pathFinder(findCave(startCaveName), [], pathsFound, false);

console.log("Result:", pathsFound.length);

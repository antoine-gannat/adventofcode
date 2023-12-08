import { read } from "../utils";

interface IMap {
  ranges: {
    source: number;
    destination: number;
    range: number;
  }[];
  source: string;
  destination: string;
}

function mapSourceToDestination(source: number, map: IMap): number {
  // find a range that matches the source
  const range = map.ranges.find(
    (range) => source >= range.source && source < range.source + range.range
  );
  // if no range is found, keep the source
  if (!range) {
    return source;
  }

  // if the range is found, calculate the destination
  const distanceToRangeStart = source - range.source;

  return range.destination + distanceToRangeStart;
}

const fileContent = read(5).split("\n");

const maps: IMap[] = [];
const seeds = fileContent[0].match(/(\d+)/gm).map(Number);

let currentMap = -1;

// parse the input
fileContent.slice(1).forEach((line) => {
  if (line.length === 0) {
    return;
  }
  const headerMatch = /(\w+)-to-(\w+)/gm.exec(line);
  if (headerMatch) {
    const [_, source, destination] = headerMatch;
    maps[++currentMap] = {
      ranges: [],
      source,
      destination,
    };
    return;
  }
  const rangeMatch = /(\d+) (\d+) (\d+)/gm.exec(line);
  if (rangeMatch) {
    const [_, destination, source, range] = rangeMatch.map(Number);
    maps[currentMap].ranges.push({
      source,
      destination,
      range,
    });
    return;
  }
});

let lowestLocation = Number.MAX_SAFE_INTEGER;
// go through each seed
for (let i = 0; i + 1 < seeds.length; i += 2) {
  const start = seeds[i];
  const end = start + seeds[i + 1];
  for (let seed = start; seed <= end; seed++) {
    let currentValue = seed;
    maps.forEach((map) => {
      currentValue = mapSourceToDestination(currentValue, map);
    });
    if (currentValue < lowestLocation) {
      lowestLocation = currentValue;
    }
  }
}

console.log("Result", { lowestLocation });

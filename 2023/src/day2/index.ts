import { read } from "../utils";

interface ICubes {
  color: string;
  count: number;
}

interface ISet {
  cubes: ICubes[];
}

const fileContent = read(2).split("\n");

const regex = new RegExp(/((\d+) (\w+)[;,$]*)/gim);

let match;

let total = 0;

// for each game
fileContent.forEach((line) => {
  const sets: ISet[] = [];
  let currentSet: ISet = { cubes: [] };

  while ((match = regex.exec(line)) !== null) {
    currentSet.cubes.push({ color: match[3], count: Number(match[2]) });
    if (match[0].includes(";")) {
      sets.push(currentSet);
      currentSet = { cubes: [] };
    }
  }
  sets.push(currentSet);

  const maxes: Record<string, number> = {};

  sets.forEach((set) =>
    set.cubes.forEach((cube) => {
      if (maxes[cube.color] === undefined) {
        maxes[cube.color] = cube.count;
      } else if (cube.count > maxes[cube.color]) {
        maxes[cube.color] = cube.count;
      }
    })
  );
  total += Object.values(maxes).reduce((acc, curr) => acc * curr, 1);
});

console.log(total);

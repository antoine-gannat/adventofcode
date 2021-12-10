import { readInput } from "../../utils/readInput";

const fishes = readInput().split(",").map(Number);

const nbDays = 256;
const adultReproDuration = 7;
const childReproDuration = 9;

const reproPlannedPerDay: { [day: number]: { child: number; adult: number } } =
  {};
// init
for (let day = 0; day < nbDays; day++) {
  reproPlannedPerDay[day] = { child: 0, adult: 0 };
}

fishes.forEach((f) => reproPlannedPerDay[f].adult++);

let totalRepro = fishes.length;

for (let day = 0; day < nbDays; day++) {
  const reproPlanned = reproPlannedPerDay[day];
  const totalDayRepro = reproPlanned.adult + reproPlanned.child;
  if (!totalDayRepro) {
    continue;
  }
  totalRepro += totalDayRepro;
  // plan child next repro
  // plan adult next repro
  const nextChildReproDay = day + childReproDuration;
  const nextAdultReproDay = day + adultReproDuration;
  if (nextChildReproDay < nbDays) {
    reproPlannedPerDay[nextChildReproDay].child += totalDayRepro;
  }
  if (nextAdultReproDay < nbDays) {
    reproPlannedPerDay[nextAdultReproDay].adult += totalDayRepro;
  }
}

console.log({ totalRepro });

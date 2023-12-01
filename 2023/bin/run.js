const path = require("path");
const fs = require("fs");

if (process.argv.length < 3) {
  console.error("Usage: yarn start <day>");
  process.exit(1);
}

const day = process.argv[2];

const dayPath = path.join(__dirname, "..", "lib", "day" + day, "index.js");

if (!fs.existsSync(dayPath)) {
  console.error("Day not found:", dayPath);
  process.exit(1);
}

// run the day
require(dayPath);

const Viz                = require('viz.js');
const { Module, render } = require('viz.js/full.render.js');
const fs                 = require('fs').promises;

const viz = new Viz({ Module, render });

async function generateSVG(dotfile) {
  const dot    = await fs.readFile(dotfile, 'utf8');
  const result = await viz.renderString(dot);
  console.log(result);
}

generateSVG('client_architecture.dot');

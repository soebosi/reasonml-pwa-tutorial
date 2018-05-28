const Viz                = require('viz.js');
const { Module, render } = require('viz.js/full.render.js');

const viz = new Viz({ Module, render });

async function generateSVG(dot) {
  const result = await viz.renderString(dot);
  console.log(result);
}

generateSVG('digraph { a -> b }');

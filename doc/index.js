const Viz                = require('viz.js');
const { Module, render } = require('viz.js/full.render.js');
const fs                 = require('fs').promises;

const viz = new Viz({ Module, render });

async function generateSVG(name) {
  const dotfile = `${name}.dot`;
  const svgfile = `${name}.svg`;
  const dot = await fs.readFile(dotfile, 'utf8');
  try {
    const result = await viz.renderString(dot);
    await fs.writeFile(svgfile, result);
    console.log('generated', svgfile);
  } catch(e) {
    console.error(e);
  }
}

generateSVG('client_architecture');

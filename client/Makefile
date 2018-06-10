.PHONY: default

default:
	npx bsb -clean-world
	npx bsb -make-world
	NODE_ENV=production npx webpack

fmt:
	npx refmt ./src/**/*.re src/*.re --in-place

start:
	npx bsb -make-world -w

webpack:
	npx webpack -w

analyze:
	NODE_ENV=production npx webpack --json | npx webpack-bundle-size-analyzer

/**
 * @license
 * Copyright (c) 2016 The Polymer Project Authors. All rights reserved.
 * This code may only be used under the BSD style license found at http://polymer.github.io/LICENSE.txt
 * The complete set of authors may be found at http://polymer.github.io/AUTHORS.txt
 * The complete set of contributors may be found at http://polymer.github.io/CONTRIBUTORS.txt
 * Code distributed by Google as part of the polymer project is also
 * subject to an additional IP rights grant found at http://polymer.github.io/PATENTS.txt
 */

'use strict';

const del = require('del');
const gulp = require('gulp');
const gulpif = require('gulp-if');
const imagemin = require('gulp-imagemin');
const mergeStream = require('merge-stream');
const uglify = require('gulp-uglify');
const gzip = require('gulp-gzip');
const htmlMinifier = require('gulp-htmlmin');
const polymerBuild = require('polymer-build');
const HtmlSplitter = require('polymer-build').HtmlSplitter;

const swPrecacheConfig = require('./sw-precache-config.js');
const polymerJson = require('./polymer.json');
const polymerProject = new polymerBuild.PolymerProject(polymerJson);
const SourcesHtmlSplitter = new HtmlSplitter();
const dependenciesHtmlSplitter = new HtmlSplitter();

const buildDirectory = 'build';
const buildGZipDirectory = 'build-gzip';
const arduinoDataFolder = '../Arduino-ESP2866-12E/data'

/**
 * Waits for the given ReadableStream
 */
function waitFor(stream) {
  return new Promise((resolve, reject) => {
    stream.on('end', resolve);
    stream.on('error', reject);
  });
}

function build() {
  return new Promise((resolve, reject) => { // eslint-disable-line no-unused-vars
    // Okay, so first thing we do is clear the build directory
    console.log(`Deleting ${buildDirectory} directory...`);
    del([buildDirectory])
      .then(() => {

        console.log(`Deleting ${buildGZipDirectory} directory...`);
        del([buildGZipDirectory])
      .then(() => {
        // Okay, now let's get your source files
        console.log('Split Source...');
        let sourcesStream = polymerProject.sources()
          // Oh, well do you want to minify stuff? Go for it!
          // Here's how splitHtml & gulpif work
           .pipe(SourcesHtmlSplitter.split())
           .pipe(gulpif(/\.js$/, uglify()))
           .pipe(gulpif(/\.html$/, htmlMinifier(
            // {collapseWhitespace: true ,minifyCSS: true ,minifyJS:true, removeComments: true}
             )))
           .pipe(gulpif(/\.(png|gif|jpg|svg)$/, imagemin()))
          .pipe(SourcesHtmlSplitter.rejoin())
          ;

        console.log('Split Dependencies...');
        // Okay, now let's do the same to your dependencies
        let dependenciesStream = polymerProject.dependencies()
          .pipe(dependenciesHtmlSplitter.split())
          .pipe(gulpif(/\.js$/, uglify()))
          .pipe(gulpif(/\.html$/, htmlMinifier(
          //  {collapseWhitespace: true ,minifyCSS: true ,minifyJS:true, removeComments: true}
            )))
          .pipe(dependenciesHtmlSplitter.rejoin())
          ;

        // Okay, now let's merge them into a single build stream
        let buildStream = mergeStream(sourcesStream, dependenciesStream)
          .once('data', () => {
            console.log('Analyzing build dependencies...');
          });

        // If you want bundling, pass the stream to polymerProject.bundler.
        // This will bundle dependencies into your fragments so you can lazy
        // load them.
        buildStream = buildStream.pipe(polymerProject.bundler);

        // Okay, time to pipe to the build directory
        buildStream = buildStream.pipe(gulp.dest(buildDirectory));

        // waitFor the buildStream to complete
        return waitFor(buildStream);
      })
      .then(() => {
        // Okay, now let's generate the Service Worker
        console.log('Generating the Service Worker...');
        return polymerBuild.addServiceWorker({
          project: polymerProject,
          buildRoot: buildDirectory,
          bundled: true,
          swPrecacheConfig: swPrecacheConfig
        });
      })
      .then(() => {

        console.log(`Deleting ${buildDirectory} directory...`);
        del([buildDirectory+"/bower_components"])
          .then(() => {
              console.log(`Copy aditional files...`);
              gulp.src('./bower_components/webcomponentsjs/webcomponents-lite.min.js')
              .pipe(gulp.dest(buildDirectory)) ;

                console.log(`Compress files...`);
                var compress = gulp.src(buildDirectory + '/**')
                .pipe(gzip({ skipGrowingFiles : true }))
                .pipe(gulp.dest(buildGZipDirectory))

                .on('end', function(){                      
                      Uncompressed();
                      MoveGzipToData();                                // You did it!
                      console.log('Build complete!'); //console.log('Build complete!');
                      resolve();
                      })
          });
         });
  });
  });
}


function Uncompressed(){
      console.log(`Copy aditional non compresed files...`);
      del([buildGZipDirectory+"/bower.json.gz"]).then(() => {
          gulp.src(buildDirectory + '/bower.json')
          .pipe(gulp.dest(buildGZipDirectory));
      });
      del([buildGZipDirectory+"/manifest.json.gz"]).then(() => {
          gulp.src(buildDirectory + '/manifest.json')
          .pipe(gulp.dest(buildGZipDirectory));
      });
      del([buildGZipDirectory+"/webcomponents-lite.min.js.json.gz"]).then(() => {
          gulp.src('./bower_components/webcomponentsjs/webcomponents-lite.min.js')
          .pipe(gulp.dest(buildGZipDirectory));
      });
}

function MoveGzipToData(){
  console.log(`Copy to arduino data folder...`);
  del([arduinoDataFolder]).then(() => {
      gulp.src(buildGZipDirectory)
      .pipe(gulp.dest(arduinoDataFolder));
  });
}
gulp.task('build', build);

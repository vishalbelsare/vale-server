'use strict'

const gulp = require('gulp')
const minifyCSS = require('gulp-csso')
const uglifyJS = require('gulp-uglify')
const babel = require('gulp-babel')
const nunjucksRender = require('gulp-nunjucks-render')

gulp.task('css', function () {
  return gulp.src('./css/*.css')
    .pipe(minifyCSS())
    .pipe(gulp.dest('../build/dashboard/css'))
})

gulp.task('js', function () {
  return gulp.src('./js/*.js', { sourcemaps: false })
    .pipe(babel({
      presets: ['@babel/preset-env']
    }))
    .pipe(uglifyJS())
    .pipe(gulp.dest('../build/dashboard/js'))
})

gulp.task('vendor', function () {
  return gulp.src('./vendor/**/*')
    .pipe(gulp.dest('../build/dashboard/vendor'))
})

gulp.task('img', function () {
  return gulp.src('./img/**/*')
    .pipe(gulp.dest('../build/dashboard/img'))
})

gulp.task('nunjucks', function () {
  return gulp.src('./pages/**/*.+(njk)')
    .pipe(nunjucksRender({
      path: ['./templates']
    }))
    .pipe(gulp.dest('../build/dashboard/'))
})

gulp.task('icon', function () {
  return gulp.src('./favicon.ico')
    .pipe(gulp.dest('../build/dashboard/'))
})

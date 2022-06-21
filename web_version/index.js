
import * as THREE from 'three';
import { OrbitControls } from 'OrbitControls';
import { GUI } from "gui"
import { Line2 } from "line2"
import { LineMaterial } from "lineMaterial"
import { LineGeometry } from "lineGeometry"

let canvas, camera, scene, renderer, controls;

const sprite = new THREE.TextureLoader().load('sprite.png');

init_scene();

function init_scene() {

  canvas = document.querySelector(".webgl");

  const size = {
    width: window.innerWidth,
    height: window.innerHeight,
  };


  renderer = new THREE.WebGL1Renderer({
    canvas: canvas,
    antialias: true
  });
  renderer.setSize(size.width, size.height);
  renderer.setPixelRatio(window.devicePixelRatio);
  renderer.setClearColor(0x000000, 1.0);
  renderer.shadowMap.enabled = true;
  document.body.appendChild(renderer.domElement);

  scene = new THREE.Scene();

  camera = new THREE.PerspectiveCamera(45, size.width / size.height, 1, 500);
  camera.position.set(0, 0, 100);
  camera.lookAt(0, 0, 0);
  scene.add(camera);

  controls = new OrbitControls(camera, renderer.domElement);
}

var pointArr = [
  -100, 0, 0,
  -100, 100, 0,
  0, 0, 0,
  100, 100, 0,
  100, 0, 0
];
var colors = []
var col = new THREE.Color();
for (let i = 0; i < 5; i++) {
  const t = i / 5;
  col.setHSL(t, 1.0, 0.5);
  colors.push(col.r, col.g, col.b);
}

var lineGeometry = new LineGeometry();
lineGeometry.setPositions(pointArr);
lineGeometry.setColors(colors);

var lineMaterial = new LineMaterial({
  color: 0xffffff,
  linewidth: 5,
  vertexColors: true
})
lineMaterial.resolution.set(window.innerWidth, window.innerHeight)
var line = new Line2(lineGeometry, lineMaterial)
line.computeLineDistances()
scene.add(line)

var pointGeometry = new THREE.BufferGeometry();
pointGeometry.setAttribute('position', new THREE.BufferAttribute(new Float32Array(pointArr), 3));
pointGeometry.setAttribute('color', new THREE.Float32BufferAttribute(new Float32Array(colors), 3));
const pointMaterial = new THREE.PointsMaterial({
  size: 20,
  sizeAttenuation: true,
  map: sprite,
  transparent: true,
  vertexColors: true
});
const pointCloud = new THREE.Points(pointGeometry, pointMaterial);
scene.add(pointCloud);


function animate() {
  requestAnimationFrame(animate);
  renderer.render(scene, camera);
}

animate()
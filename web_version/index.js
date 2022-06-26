
import * as THREE from 'three';
import { OrbitControls } from 'OrbitControls';
import { Line2 } from "line2"
import { LineMaterial } from "lineMaterial"
import { LineGeometry } from "lineGeometry"

var cover = document.querySelector(".cover")
cover.parentNode.removeChild(cover)

let canvas, camera, scene, renderer, controls;

const sprite = new THREE.TextureLoader().load('sprite.png');

init_scene();

const color_list = document.querySelector('.colorlist')
var pickr_list = [];

function add_color() {
  const newPickr = document.createElement('div');
  color_list.appendChild(newPickr);
  var default_color = '#42445A';
  if(pickr_list.length > 0)
    default_color = pickr_list[pickr_list.length-1].get_color();
  var new_point = new ColorPoint(newPickr, default_color);
  pickr_list.push(new_point);
  // console.log(lineGeometry);
}

function remove_color() {
  if (pickr_list.length > 0) {
    pickr_list[pickr_list.length - 1].remove_element();
    pickr_list.splice(pickr_list.length - 1, 1);
  }
}

function init_scene() {

  canvas = document.querySelector(".webgl");

  var inwidth = window.innerWidth;
  var inheight = window.innerHeight;

  const size = {
    width: inwidth * 0.5, //window.innerWidth,
    height: inheight * 0.5, // window.innerHeight,
  };

  renderer = new THREE.WebGL1Renderer({
    canvas: canvas,
    antialias: true
  });
  renderer.setSize(size.width, size.height);
  renderer.setPixelRatio(window.devicePixelRatio);
  renderer.setClearColor(0x171723, 1.0);
  renderer.shadowMap.enabled = true;
  document.body.appendChild(renderer.domElement);

  scene = new THREE.Scene();

  camera = new THREE.PerspectiveCamera(50, size.width / size.height, 1, 1000);
  camera.position.set(180, 120, -90);
  camera.lookAt(0, 0, 0);
  scene.add(camera);

  controls = new OrbitControls(camera, renderer.domElement);
}

var pointArr = [];
var colors = []

function updateColorPoint() {
  pointArr.splice(0, pointArr.length);
  colors.splice(0, colors.length);
  for (let i = 0; i < pickr_list.length; i++) {
    pointArr.push(pickr_list[i].LAB[0], pickr_list[i].LAB[1], pickr_list[i].LAB[2]);
    const RGB = pickr_list[i].pickr._color.toRGBA();
    colors.push(RGB[0] / 255, RGB[1] / 255, RGB[2] / 255);
  }
}

add_color();

updateColorPoint();

var lineGeometry = new LineGeometry();
lineGeometry.setPositions(pointArr);
lineGeometry.setColors(colors);

var lineMaterial = new LineMaterial({
  color: 0xffffff,
  linewidth: 5,
  vertexColors: true
});
lineMaterial.resolution.set(window.innerWidth, window.innerHeight);
var line = new Line2(lineGeometry, lineMaterial);
line.computeLineDistances();
scene.add(line);

var pointGeometry = new THREE.BufferGeometry();
pointGeometry.setAttribute('position', new THREE.BufferAttribute(new Float32Array(pointArr), 3));
pointGeometry.setAttribute('color', new THREE.Float32BufferAttribute(new Float32Array(colors), 3));
pointGeometry.dynamic
const pointMaterial = new THREE.PointsMaterial({
  size: 20,
  sizeAttenuation: true,
  map: sprite,
  transparent: true,
  vertexColors: true
});
const pointCloud = new THREE.Points(pointGeometry, pointMaterial);
scene.add(pointCloud);

function fun(i, mask, n, memo, trace, dist) {
  if (mask == ((1 << i) | 1)) {
    memo[i][mask] = dist[0][i];
    trace[i][mask] = 0;
    return dist[0][i];
  }

  if (memo[i][mask] != -1)
    return memo[i][mask];

  var res = 10 ** 9;
  var node = 0;
  for (let j = 0; j < n; j++) {
    if ((mask & (1 << j)) != 0 && j != i && j != 0)
      var tmp = fun(j, mask & (~(1 << i)), n, memo, trace, dist) + dist[j][i];
    if (tmp < res) {
      res = tmp;
      node = j;
    }
  }
  memo[i][mask] = res;
  trace[i][mask & (~(1 << i))] = node;
  return res;
}

function reorder() {
  var n = pickr_list.length;
  var color_info = [];
  var order = [];
  var dist = new Array(n);
  for (let i = 0; i < n; i++) {
    color_info.push(pickr_list[i].get_color());
    dist[i] = new Array(n);
    for (let j = 0; j < n; j++) {
      if (i == j)
        dist[i][j] = 0;
      else
        dist[i][j] = LAB_Distance(pickr_list[i].LAB, pickr_list[j].LAB);
    }
  }

  // n=4;
  // dist = [[0, 10, 15, 20], [10, 0, 25, 25], [15, 25, 0, 30], [20, 25, 30, 0]];
  // Traveling Salesman Solver
  let memo = new Array(n);
  let trace = new Array(n);
  for (let i = 0; i < n; i++) {
    memo[i] = new Array(1 << n);
    memo[i].fill(-1);
    trace[i] = new Array(1 << n);
    trace[i].fill(-1);
  }

  var ans = 10 ** 9;
  var mask = (1 << n) - 1;
  for (let i = 0; i < n; i++) {
    var tmp = fun(i, mask, n, memo, trace, dist) + dist[i][0];
    if (tmp < ans) {
      ans = tmp;
      memo[0][mask] = ans;
      trace[0][mask] = i;
    }
  }

  mask = (1 << n) - 1;
  var node = 0;
  var trace_back = [0];
  while (mask != 1) {
    var nxt = trace[node][mask];
    trace_back.push(nxt);
    mask = mask & (~(1 << nxt));
    node = nxt;
  }

  var max_edge = dist[trace_back[0]][trace_back[1]];
  var max_edge_index = 0;
  for (let i = 1; i < n; i++) {
    var tmp;
    if (i < n - 1)
      tmp = dist[trace_back[i]][trace_back[i + 1]];
    else
      tmp = dist[trace_back[i]][trace_back[0]];
    if (tmp > max_edge) {
      max_edge = tmp;
      max_edge_index = i;
    }
  }

  var index = max_edge_index + 1;
  if (index >= n)
    index = 0;
  while (index != max_edge_index) {
    order.push(trace_back[index]);
    index += 1;
    if (index >= n) index = 0;
  }
  order.push(trace_back[max_edge_index]);
  console.log(order);

  
    for(let i=0;i<pickr_list.length;i++){
      pickr_list[i].set_color(color_info[order[i]]);
    }
}

document.getElementById("add").onclick = add_color;
document.getElementById("sub").onclick = remove_color;
document.getElementById("reorder").onclick = reorder;


function animate() {
  requestAnimationFrame(animate);
  updateColorPoint();
  lineGeometry.setColors(colors);
  lineGeometry.setPositions(pointArr);
  lineGeometry._maxInstanceCount = pickr_list.length - 1;
  line.computeLineDistances();
  pointGeometry.setAttribute('position', new THREE.BufferAttribute(new Float32Array(pointArr), 3));
  pointGeometry.setAttribute('color', new THREE.Float32BufferAttribute(new Float32Array(colors), 3));
  renderer.render(scene, camera);
}

animate()
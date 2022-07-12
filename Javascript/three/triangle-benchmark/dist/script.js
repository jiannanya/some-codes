// 判断浏览器是否可以渲染3D场景
if (!Detector.webgl) Detector.addGetWebGLMessage();

var controls, stats;

var camera, scene, renderer;

var light;

var param;

const clearColor = 0x000000; // 0xFFFFFF

const near = 1;
const far = 3500;

function initCamera() {
    camera = new THREE.PerspectiveCamera(27, width / height, near, far);
    camera.position.z = 2750;
    scene.add(camera);
}

function initLight() {
    scene.add(new THREE.AmbientLight(0x444444));
    light = new THREE.DirectionalLight(0xffffff, 0.5);
    light.position.set(1, 1, 1);
    scene.add(light);

    var light2 = new THREE.DirectionalLight(0xffffff, 1.5);
    light2.position.set(0, -1, 0);
    scene.add(light2);
}

function initGUI() {
    var ParamObj = function () {
        this.repeat = 4;
        this.wrap = 1000;
    };
    param = new ParamObj();
    const gui = new dat.GUI();
    // gui.add(param, "repeat", -100, 100).name('纹理重复')
    // gui.add(param, "wrap", 1000, 1002).name('纹理环绕').step(1)
}

// 初始化物体
function initObject() {
    // 创建三角形
    var triangles = 1600000;

    var geometry = new THREE.BufferGeometry();
    // 生成160万个三角形所需顶点的存储空间，一个三角形有三个顶点，一个顶点有xyz三个变量
    var positions = new Float32Array(triangles * 3 * 3);
    // 这里是每个顶点一个法线，也可以一个面一个法线
    var normals = new Float32Array(triangles * 3 * 3);
    // 每个顶点一种颜色
    var colors = new Float32Array(triangles * 3 * 3);

    var color = new THREE.Color();

    var n = 800, n2 = n / 2; // 限定三角形出现的范围是[-400,400]这么一个立方体中，n2表示直径的一半
    var d = 12, d2 = d / 2; // 限定三角形的大小为12，d2表示边长的一半(假如有半径)

    var pA = new THREE.Vector3();
    var pB = new THREE.Vector3();
    var pC = new THREE.Vector3();

    var cb = new THREE.Vector3();
    var ab = new THREE.Vector3();

    for (var i = 0; i < positions.length; i += 9) {
        // 通过随机数生成点的位置

        // 生成一个顶点,范围是[-400,400]
        var x = Math.random() * n - n2;
        var y = Math.random() * n - n2;
        var z = Math.random() * n - n2;

        // 根据随机生成的顶点，随机生成三个点，供组成三角形
        // 随机产生a,b,c三个点
        // 点的位置再加上三角形的大小
        var ax = x + Math.random() * d - d2;
        var ay = y + Math.random() * d - d2;
        var az = z + Math.random() * d - d2;

        var bx = x + Math.random() * d - d2;
        var by = y + Math.random() * d - d2;
        var bz = z + Math.random() * d - d2;

        var cx = x + Math.random() * d - d2;
        var cy = y + Math.random() * d - d2;
        var cz = z + Math.random() * d - d2;
        // a点
        positions[i] = ax;
        positions[i + 1] = ay;
        positions[i + 2] = az;
        // b点
        positions[i + 3] = bx;
        positions[i + 4] = by;
        positions[i + 5] = bz;
        // c点
        positions[i + 6] = cx;
        positions[i + 7] = cy;
        positions[i + 8] = cz;

        // 把a,b,c点放到向量中
        pA.set(ax, ay, az);
        pB.set(bx, by, bz);
        pC.set(cx, cy, cz);
        // 向量减法计算向量，是为了计算垂直于三角形的向量
        cb.subVectors(pC, pB)
        ab.subVectors(pA, pB)
        cb.cross(ab) // 得到正交向量，垂直于两个向量组成的平面，也就是法线
        // 向量的归一化
        cb.normalize()

        // 存储法线坐标
        var nx = cb.x
        var ny = cb.y
        var nz = cb.z
        // a顶点法线
        normals[i] = nx
        normals[i + 1] = ny
        normals[i + 2] = nz
        // b顶点法线
        normals[i + 3] = nz
        normals[i + 4] = nz
        normals[i + 5] = nz
        // c顶点法线
        normals[i + 6] = nz
        normals[i + 7] = nz
        normals[i + 8] = nz

        // 为每个顶点赋值颜色
        // x / n得到范围[-0.5,0.5],加0.5得到[0,1]
        var vx = (x / n) + 0.5
        var vy = (y / n) + 0.5
        var vz = (z / n) + 0.5

        color.setRGB(vx, vy, vz)

        colors[i] = color.r
        colors[i + 1] = color.g
        colors[i + 2] = color.b

        colors[i + 3] = color.r
        colors[i + 4] = color.g
        colors[i + 5] = color.b

        colors[i + 6] = color.r
        colors[i + 7] = color.g
        colors[i + 8] = color.b
    }

    geometry.addAttribute('position', new THREE.BufferAttribute(positions, 3))
    geometry.addAttribute('normal', new THREE.BufferAttribute(normals, 3))
    geometry.addAttribute('color', new THREE.BufferAttribute(colors, 3))
    // 计算几何体的包围盒
    geometry.computeBoundingSphere();

    var material = new THREE.MeshPhongMaterial({
        color: 0xaaaaaa,
        specular: 0xffffff,
        shininess: 250,
        side: THREE.DoubleSide,
        vertexColors: THREE.VertexColors
    })

    mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);
}

/* 数据更新 */
function update() {
    var time = Date.now() * 0.001;
    mesh.rotation.x = time * 0.25
    mesh.rotation.y = time * 0.5
}

// 颜色选择器
window.colorPickerUpdate = function (picker, selector) {
    const hex = "0x" + picker.toHEXString().replace(/#/, "");
    light.color.setHex(hex);
};

/* --------------- 不需要经常修改的部分 --------------------- */

function animation() {
    renderer.render(scene, camera);
    requestAnimationFrame(animation);
    stats.update();
    controls.update();
    update();
}

function threeStart() {
    // 初始化场景
    initScene();
    // 初始化渲染器
    initRenderer();
    // 初始化相机
    initCamera();
    // 初始化灯光
    initLight();
    // 初始化物体
    initObject();

    // 初始化布控球
    initControls();
    // 初始化GUI界面
    initGUI();
    // 初始化性能监控
    initStats();
    // 初始化颜色选择器
    initColorPicker();

    // 渲染循环
    animation();
    /* 监听事件 */
    window.addEventListener("resize", onWindowResize, false);
}

function initRenderer() {
    width = document.getElementById("canvas-frame").clientWidth;
    height = document.getElementById("canvas-frame").clientHeight;
    renderer = new THREE.WebGLRenderer({
        antialias: false,
    });
    renderer.setSize(width, height);
    document.getElementById("canvas-frame").appendChild(renderer.domElement);
    renderer.setClearColor(scene.fog.color);
    renderer.setPixelRatio(window.devicePixelRatio)

    renderer.gammaInput = true;
    renderer.gammaOutput = true;
}

function initScene() {
    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0x050505, 2000, 3500);
}

var stats;
function initStats() {
    stats = new Stats();
    stats.showPanel(0); // 0: fps, 1: ms, 2: mb, 3+: custom
    // 将stats的界面对应左上角
    stats.domElement.style.position = "absolute";
    stats.domElement.style.left = "0px";
    stats.domElement.style.top = "0px";
    document.body.appendChild(stats.dom);
}

function initColorPicker() {
    const colorPicker = document.createElement("div");
    colorPicker.id = "color-picker";
    document.body.appendChild(colorPicker);
    const html = `<input
      data-jscolor="{value:'#FF0000', alpha:1}"
      onChange="colorPickerUpdate(this.jscolor, '#pr3')"
    />`;
    const colorPickerDom = document.querySelector("#color-picker");
    colorPickerDom.innerHTML = html;

    jscolor.install(colorPickerDom);

    jscolor.presets.default = {
        width: 141, // make the picker a little narrower
        position: "top", // position it to the right of the target
        previewPosition: "right", // display color preview on the right
        previewSize: 40, // make the color preview bigger
        format: "hex",
        previewSize: 40,
        closeButton: true,
        closeText: "关闭",
        palette: [
            "#000000",
            "#7d7d7d",
            "#870014",
            "#ec1c23",
            "#ff7e26",
            "#fef100",
            "#22b14b",
            "#00a1e7",
            "#3f47cc",
            "#a349a4",
            "#ffffff",
            "#c3c3c3",
            "#b87957",
            "#feaec9",
            "#ffc80d",
            "#eee3af",
            "#b5e61d",
            "#99d9ea",
            "#7092be",
            "#c8bfe7",
        ],
    };
}

function initControls() {
    controls = new THREE.TrackballControls(camera);

    controls.rotateSpeed = 0.2; // 旋转速度
    controls.zoomSpeed = 0.2; // 缩放速度
    controls.panSpeed = 0.1; // 平controls
    controls.staticMoving = false; // 静止移动，为 true 则没有惯性
    controls.dynamicDampingFactor = 0.2; // 阻尼系数 越小 则滑动越大

    controls.minDistance = near; // 最小视角
    controls.maxDistance = far; // 最大视角 Infinity 无穷大
}

/* 窗口变动触发 */
function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}


window.addEventListener("load", function (event) {
    console.log("文档以及它包含的资源都已加载完成");
    // console.log(THREE)
    threeStart();
});
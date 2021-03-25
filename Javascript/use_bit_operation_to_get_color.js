/**
 * 巧妙使用位运算获取RGB颜色值各个部分
 */

// 如果编写通用的材质加载器需要枚举所有的材质，这里没有列举完
let typeAPI = {
    MeshLambertMaterial: THREE.MeshLambertMaterial,
    MeshBasicMaterial: THREE.MeshBasicMaterial,
    MeshPhongMaterial: THREE.MeshPhongMaterial,
    PointsMaterial: THREE.PointsMaterial,
  }
  // 创建一个文件加载器，该加载器是对异步加载的封装
  let loader = new THREE.FileLoader();
  loader.load('material.json', function(elem) {
    console.log(elem);// 查看加载返回的内容
    let obj = JSON.parse(elem);// 字符串转JSON对象
    console.log(obj);// 查看转化结果
    let geometry = new THREE.BoxGeometry(100, 100, 100);
    /**
     * 解析材质数据
     */
    // 根据type的值判断调用threejs的哪一个API
    let material = new typeAPI[obj.type]();
    // 从obj.color中提取颜色
    // 16711935对应颜色0xFF00FF  255对应颜色0x0000FF
    material.color.r = (obj.color >> 16 & 255) / 255; //获取颜色值R部分
    material.color.g = (obj.color >> 8 & 255) / 255; //获取颜色值G部分
    material.color.b = (obj.color & 255) / 255; //获取颜色值B部分
  
    let mesh = new THREE.Mesh(geometry, material); //网格模型对象Mesh
    scene.add(mesh); //网格模型添加到场景中
  })
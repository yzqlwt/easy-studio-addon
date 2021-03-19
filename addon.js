var addon = require('bindings')('addon.node')

addon.setSkinPath("res\\ui\\games\\game411\\skin01\\")
addon.setTPPath("C:\\Users\\yzqlwt\\Documents\\easy-studio4\\assets\\Release\\assets\\TexturePacker.exe")

addon.setCCSPath("C:\\Users\\yzqlwt\\Documents\\WorkSpace\\cocos-ui\\mangomath-ui\\CocosProject.ccs")
addon.setSkinId(1921)

// // addon.gotoFolder("C:/Users/yzqlwt/Documents/WorkSpace/cocos-ui/mangomath-ui/cocosstudio/res/ui/games/game411/skin01/")

// addon.tiny()
// console.log("tiny", addon.getNeedTinyFiles())
addon.generateCSD()
// addon.package()
// console.log("package")
// addon.upload()
// console.log("upload")
// console.log(addon.download())
// addon.parse()

// addon.download()



// // console.log(addon.getSkinFullPath())
// // console.log(addon.getFolder("C:/Users/yzqlwt/Documents"))
// // console.log(addon.handleImages())
// setInterval(() => {
//     console.log(addon.getClipboardFiles())
// }, 3000)
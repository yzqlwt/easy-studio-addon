var addon = require('bindings')('addon.node')

addon.setSkinPath("res/ui/games/game411/skin01")
addon.setTPPath("C:/Users/yzqlwt/easy-studio-addon/build/Release/assets/TexturePacker.exe")

addon.setCCSPath("C:/Users/yzqlwt/Documents/WorkSpace/cocos-ui/mangomath-ui/CocosProject.ccs")


// // addon.gotoFolder("C:/Users/yzqlwt/Documents/WorkSpace/cocos-ui/mangomath-ui/cocosstudio/res/ui/games/game411/skin01/")
const EventEmitter = require('events').EventEmitter

const emitter = new EventEmitter()

emitter.on('error', () => {
    console.log('### START ...')
})
emitter.on('tiny', (evt) => {
    console.log(evt);
})

emitter.on('data', (data) => {
    console.log(data)
})

addon.package(emitter.emit.bind(emitter))


// // console.log(addon.getSkinFullPath())
// // console.log(addon.getFolder("C:/Users/yzqlwt/Documents"))
// // console.log(addon.handleImages())
// setInterval(() => {
//     console.log(addon.getClipboardFiles())
// }, 3000)
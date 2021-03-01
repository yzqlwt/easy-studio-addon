var addon = require('bindings')('addon.node')

addon.setSkinPath("res/ui/games/game207/skin01")

addon.setCCSPath("C:\\Users\\yzqlwt\\Documents\\WorkSpace\\cocos-ui-back\\mangomath-ui\\CocosProject.ccs")



const EventEmitter = require('events').EventEmitter

const emitter = new EventEmitter()

emitter.on('error', () => {
    console.log('### START ...')
})
emitter.on('tiny', (evt) => {
    console.log(evt);
})

emitter.on('warn', (data) => {
    console.log(data)
})

addon.package(emitter.emit.bind(emitter))


console.log(addon.getSkinFullPath())

// console.log(addon.handleImages())
var addon = require('bindings')('addon.node')

addon.setSkinPath("res/ui/games/game275/skin01")

addon.setCCSPath("C:\\Users\\yzqlwt\\Downloads\\mangomath-ui-yzq\\CocosProject.ccs")



const EventEmitter = require('events').EventEmitter

const emitter = new EventEmitter()

emitter.on('error', () => {
    console.log('### START ...')
})
emitter.on('warn', (evt) => {
    console.log(evt);
})

emitter.on('steps', (data) => {
    console.log(data)
})

addon.package(emitter.emit.bind(emitter))


console.log(addon.getSkinFullPath())

// console.log(addon.handleImages())
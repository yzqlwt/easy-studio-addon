var addon = require('bindings')('addon.node')

addon.setSkinPath("res/ui/games/game275/skin01")

addon.setCCSPath("C:\\Users\\yzqlwt\\Downloads\\mangomath-ui-yzq\\CocosProject.ccs")

console.log(addon.getSkinFullPath())

console.log(addon.handleImages())
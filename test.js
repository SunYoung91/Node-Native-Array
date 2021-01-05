// hello.js
const addon = require('./build/Release/native-array');
console.time("Create")
const MaxValue = 6555555
let array = new addon.NativeArray(MaxValue)
console.timeEnd("Create")


console.time("SetNative")
for (let i = 0 ; i < MaxValue ; i++){
	array.set(i,{name:"test",age:1})
}
console.timeEnd("SetNative")

console.time("SetJSArray")
let jsArray = []
for (let i = 0 ; i < MaxValue ; i++){
	jsArray.push({name:"test",age:1})
}
console.timeEnd("SetJSArray")

console.time("Native Array")
// for (let i = 0 ;i < MaxValue;i++ ){
// 	for(let j = i + 1; j < MaxValue ;j++){
// 		let obj1 = array.get(i)
// 		let obj2 = array.get(j)
// 		if (obj1.age > obj2.age){
// 			array.set(i,obj2)
// 			array.set(j,obj1)
// 		}
// 	}
// }

let result = 0
for(let i = 0 ; i < MaxValue ;i++){
	let obj = array.get(i)
	result += obj.age
}
console.timeEnd("Native Array")

console.time("JSArray")
// for (let i = 0 ;i < MaxValue;i++ ){
// 	for(let j = i + 1; j < MaxValue ;j++){
// 		let obj1 = jsArray[i]
// 		let obj2 = jsArray[j]
// 		if (obj1.age > obj2.age){
// 			jsArray[i]=obj2
// 			jsArray[j]=obj1
// 		}
// 	}
// }

 result = 0
for(let i = 0 ; i < MaxValue ;i++){
	let obj = jsArray[i]
	result += obj.age
}
console.timeEnd("JSArray")




// console.log(array)
// let a = array.set(6,{name:"李昀",age:30})
// let b = array.set(1,{name:"你爹",age:30})
// console.log("result",a,b)
// setInterval(()=>{
// 	console.log(0,array.get(0))
// 	console.log(1,array.get(1))
// },1000)
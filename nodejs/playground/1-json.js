const fs = require('fs')

// const book = {
//     title: 'Ego is the Enemy',
//     author: 'Ryan Holiday'
// }

// const json_book =JSON.stringify(book)
// console.log(json_book)

// const parsed_json_book = JSON.parse(json_book)
// console.log(parsed_json_book.author)

// fs.writeFileSync('1-json.json', json_book)

// const databuffer = fs.readFileSync('1-json.json')
// const dataJson = databuffer.toString()
// console.log(dataJson)


const readBuffer = fs.readFileSync('1-json.json')
const readData = readBuffer.toString()
const data = JSON.parse(readData)
data.name = "Muthu"
data.age = "25"
const data_json = JSON.stringify(data)
console.log(data_json)
fs.writeFileSync('1-json.json', data_json)

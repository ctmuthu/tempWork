const fs = require('fs')
const chalk = require('chalk')

const getNotes = () => 'The notes is...'

const addNote = (title,body) => {
    const notes = loadNote()
    const duplicateNotes = notes.filter((note) => note.title === title)

    if(duplicateNotes.length === 0) {
       notes.push({
            title: title,
            body: body
        })
        saveNote(notes)
        console.log(chalk.green.inverse("New note taken"))
    }
    else {
        console.log(chalk.red.inverse("No notes taken"))
    }
}

const removeNote = (title) => {
    const notes = loadNote()
    const existingNotes = notes.filter((note) => note.title !== title)
    if (notes.length > existingNotes.length) {
        saveNote(existingNotes)
        console.log(chalk.green.inverse("Note has been removed"))
    }
    else {
        console.log(chalk.red.inverse("Note not available"))
    }

}

const listNotes = () => {
    const notes = loadNote()

    console.log(chalk.inverse('Your notes'))

    notes.forEach((note) => {
        console.log(note.title)
    })
}

const saveNote = (notes) =>{
    const dataJson = JSON.stringify(notes)
    fs.writeFileSync('notes.json',dataJson)
}

const loadNote = () => {
    try{
        const dataBuffer = fs.readFileSync('notes.json')
        const dataJson = dataBuffer.toString()
        return JSON.parse(dataJson)
    }catch (e) {
        return []
    }
}

const readNote = (title) => {
    const notes = loadNote()
    const existingNotes = notes.find((note) => note.title === title)
    if (existingNotes) {
        console.log(chalk.green.inverse("Note title: "+existingNotes.title))
        console.log(existingNotes.body)
    }
    else {
        console.log(chalk.red.inverse("Note not available"))
    }

}

module.exports = {
    getNotes: getNotes,
    addNote: addNote,
    removeNote: removeNote,
    listNotes: listNotes,
    readNote: readNote
}

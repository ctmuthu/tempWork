const chalk = require('chalk')
const yargs = require('yargs')
const notes = require('./notes.js')

// const command = process.argv[2]

// console.log(process.argv)

// if (command === 'add') {
//     console.log('Adding Note!')
// } else if (command === 'remove') {
//     console.log('Removing Note!')
// }
yargs.version('14.0.0')

yargs.command({
    command: 'add',
    describe: 'To add a new note',
    builder: {
        title: {
            describe: 'Note title',
            demandOption: true,
            type: 'string'
        },
        body: {
            describe: 'Note body',
            demandOption: true,
            type: 'string'
        }
    },
    handler(argv) {
        notes.addNote(argv.title,argv.body)
    }
})

yargs.command({
    command: 'remove',
    describe: 'To remove a note',
    builder: {
        title: {
            describe: 'Note title',
            demandOption: true,
            type: 'string'
        }
    },
    handler(argv) {
        notes.removeNote(argv.title)
    }
})

yargs.command({
    command: 'read',
    describe: 'To read a note',
    builder: {
        title: {
            describe: 'Note title',
            demandOption: true,
            type: 'string'
        }
    },
    handler(argv) {
        notes.readNote(argv.title)
    }
})

// Create list command
yargs.command({
    command: 'list',
    describe: 'List your notes',
    handler() {
        notes.listNotes()
    }
})


yargs.parse()
// console.log(yargs.argv)

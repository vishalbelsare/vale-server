import React, { Component } from 'react'
import Terminal from 'react-console-emulator'

const commands = {
  echo: {
    description: 'Echo a passed string.',
    fn: function () {
        console.log(arguments)
        return `${Array.from(arguments).join(' ')}`
    }
  },
  ls: {
    description: 'List directory contents.',
    fn: function (dir) {
        if (dir === 'styles') {
            return `<b>/write-good</b>  <b>/Microsoft</b>`
        } else {
            return `.vale.ini README.md <b>/styles</b>`
        }
    }
  },
  vale: {
    description: 'List directory contents.',
    fn: function (arg) {
        return "NAME: \n vale - A command-line linter for prose."
    }
  },
}

export default class ValeCLI extends Component {
  render () {
    return (
      <Terminal
        dangerMode
        commands={commands}
        welcomeMessage={'Welcome to the React terminal!'}
        promptLabel={'$'}
        style={{ backgroundColor: '#032f62' }} // #3380f3
        promptLabelStyle={{ color: '#FFFFFF' }} // Prompt label colour
        messageStyle={{ color: 'rgba(255,255,255,0.7)' }}
        inputStyle={{ color: '#FFFFFF' }} // Prompt text colour
      />
    )
  }
}

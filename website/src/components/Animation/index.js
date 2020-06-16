import React from 'react'
import Terminal from 'react-animated-term'
import useThemeContext from '@theme/hooks/useThemeContext';

import 'react-animated-term/dist/react-animated-term.css'

const termLines = [
  {
    'text': 'ls',
    'cmd': true
  },
  {
    'text': 'index.js    package.json    node_modules',
    'cmd': false
  },
  {
    'text': '',
    'cmd': true
  }
]

function ValeCLI() {
  const {isDarkTheme, setLightTheme, setDarkTheme} = useThemeContext();
  if (isDarkTheme) {
    return (
      <Terminal
        lines={termLines}
        interval={80}
      />
    )
  }
  return (
    <Terminal
      lines={termLines}
      interval={80}
      white
    />
  )
}

export default ValeCLI;

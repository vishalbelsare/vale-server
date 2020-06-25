import React, { Component } from 'react';
import {Terminal} from 'termy-the-terminal';
import './index.css';

const exampleFileSystem = {
  home: {
    type: 'FOLDER',
    children: {
      user: {
        type: 'FOLDER',
        children: null,
      },
      file1: {
        type: 'FILE',
        content: 'Contents of file 1',
        extension: 'txt',
      },
    },
  },
  docs: {
    type: 'FOLDER',
    children: null,
  },
};

export default class ValeCLI extends Component {
  render () {
    return (
    <div id="terminal-container">
      <Terminal fileSystem={exampleFileSystem} inputPrompt="$>" />
    </div>
    );
  }
}

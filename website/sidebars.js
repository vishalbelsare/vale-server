module.exports = {
  'vale-server': {
    Introduction: [
        'vale-server/install',
        'vale-server/gui',
        'vale-server/usage'
    ],
    Concepts: [
        'vale-server/concepts/styles',
        'vale-server/concepts/scoping',
        'vale-server/concepts/config',
        'vale-server/concepts/syncing',
        'vale-server/concepts/languages'
    ],
    Clients: [
        'vale-server/clients/atom',
        'vale-server/clients/chrome',
        'vale-server/clients/gdocs',
        'vale-server/clients/sublime',
        'vale-server/clients/vscode'
    ],
    'Add-ons': [
        'vale-server/add-ons/lt',
    ]
  },
  vale: {
      Introduction: [
          'vale/about',
          'vale/install',
          'vale/cli'
      ],
      Concepts: [
          'vale/styles',
          'vale/vocab',
          'vale/scoping',
          'vale/config'
      ],
      Integrations: [
          {
              type: 'link',
              label: 'GitHub Actions',
              href: 'https://github.com/errata-ai/vale-action'
          },
          {
              type: 'link',
              label: 'Visual Studio Code',
              href: 'https://github.com/errata-ai/vale-vscode'
          },
      ],
      Guides: [
          {
              type: 'link',
              label: 'Introducing Vale, an NLP-powered linter for prose',
              href: 'https://medium.com/@jdkato/introducing-vale-an-nlp-powered-linter-for-prose-63c4de31be00'
          },
          {
              type: 'link',
              label: 'The Vale Workflow',
              href: 'https://medium.com/@jdkato/the-vale-workflow-3b709fa39212'
          },
          {
              type: 'link',
              label: 'Vale & The OpenAPI Specification',
              href: 'https://medium.com/@jdkato/vale-the-openapi-specification-8a7cfae135fb'
          }
      ],
      Resources: [
          {
              type: 'link',
              label: 'Official Style Library',
              href: 'https://github.com/errata-ai/styles'
          },
          {
              type: 'link',
              label: 'Community Styles',
              href: 'https://github.com/topics/vale-linter-style'
          }
      ]
  },
};

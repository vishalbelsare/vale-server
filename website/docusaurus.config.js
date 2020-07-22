module.exports = {
  title: 'errata.ai',
  tagline: 'Innovative tools for collaborative writing',
  url: 'https://nervous-almeida-4ca728.netlify.app',
  baseUrl: '/',
  favicon: 'img/favicon.ico',
  organizationName: 'errata-ai', // Usually your GitHub org/user name.
  projectName: 'vale-server', // Usually your repo name.

  plugins: [
  ],
  scripts: [
  ],
  stylesheets: [
  ],

  themeConfig: {
    prism: {
      theme: require('prism-react-renderer/themes/github'),
      darkTheme: require('prism-react-renderer/themes/vsDark'),
      additionalLanguages: ['ini'],
    },
    // TODO:
    algolia: {
      apiKey: '89b9b22abccaf5955a5427eecb4b22e5',
      indexName: 'vale-server',
      algoliaOptions: {},
    },
    /*
    announcementBar: {
      id: 'support_us', // Any value that will identify this message.
      content: '⭐️ If you like Docusaurus, give it a star on <a target="_blank" rel="noopener noreferrer" href="https://github.com/facebook/docusaurus">GitHub</a>! ⭐️',
      backgroundColor: '#337eee', // Defaults to `#fff`.
      textColor: '#fff', // Defaults to `#000`.
    },*/
    navbar: {
      title: 'errata.ai | Docs',
      logo: {
        alt: 'My Site Logo',
        src: 'img/logo.svg',
      },
      links: [
        {to: 'blog', label: 'Blog', position: 'left'},
        {to: 'blog', label: 'Sponsor', position: 'left'},

        // {href: '/api/index.html', label: 'API', position: 'left'},

        {
          href: 'https://github.com/errata-ai',
          position: 'right',
          className: 'header-github-link',
          'aria-label': 'GitHub repository',
        },
      ],
    },
    footer: {
      links: [
        {
          title: 'About',
          items: [
            {
              label: 'Home',
              to: 'https://errata.ai',
            },
            {
              label: 'Blog',
              to: 'Blog',
            },
          ],
        },
        {
          title: 'Community',
          items: [
            {
              label: 'GitHub',
              href: 'https://github.com/errata-ai',
            },
            {
              label: 'Slack',
              href: 'https://writethedocs.slack.com/archives/CBWQQ5E57',
            },
            {
              label: 'Open Collective',
              href: 'https://opencollective.com/vale',
            },
          ],
        },
        {
          title: 'More',
          items: [
            {
              label: 'Become a Sponsor',
              to: 'blog',
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} errata.ai`,
    },
  },
  presets: [
    [
      '@docusaurus/preset-classic',
      {
        docs: {
          routeBasePath: '/',
          // It is recommended to set document id as docs home page (`docs/` path).
          homePageId: 'about',
          sidebarPath: require.resolve('./sidebars.js'),
          // Please change this to your repo.
          editUrl:
            'https://github.com/facebook/docusaurus/edit/master/website/',
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          editUrl:
            'https://github.com/facebook/docusaurus/edit/master/website/blog/',
        },
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      },
    ],
  ],
};

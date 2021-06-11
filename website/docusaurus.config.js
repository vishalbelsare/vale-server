const customFields = {
  slackUrl: 'https://writethedocs.slack.com/archives/CBWQQ5E57',
  githubOrg: 'https://github.com/errata-ai',
  valeStudio: 'https://vale-studio.errata.ai/'
}

/** @type {import('@docusaurus/types').DocusaurusConfig} */
module.exports = {
  title: 'docs.errata.ai',
  tagline: 'NLP-powered tools for automated style guide enforcement, including Vale, Vale Server, and Vale Studio.',
  url: 'https://your-docusaurus-test-site.com',
  baseUrl: '/',
  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',
  favicon: 'img/favicon.ico',
  organizationName: 'errata-ai',
  projectName: 'vale',
  themeConfig: {
    hideableSidebar: true,
    announcementBar: {
      id: 'supportus',
      backgroundColor: '#ffa48e',
      isCloseable: false,
      content:
        '⭐️ Check out the new <a target="_blank" rel="noopener noreferrer" href="https://vale-studio.errata.ai">online rule editor</a> for Vale and Vale Server! ⭐️'
    },
    algolia: {
      apiKey: '12bdf8cd642ab554d424ff89eed88ce9',
      indexName: 'errata_ai'
    },
    navbar: {
      hideOnScroll: true,
      title: 'docs.errata.ai',
      logo: {
        alt: 'errata.ai logo',
        src: 'img/logo.svg'
      },
      items: [
        {
          label: 'Docs',
          position: 'left', // or 'right'
          items: [
            {
              label: 'Vale CLI',
              to: '/vale/about/'
            },
            {
              label: 'Vale Server',
              to: '/vale-server/install/'
            },
            {
              label: 'Vale Studio',
              to: customFields.valeStudio
            }
          ]
        },
        { to: '/blog', label: 'Blog', position: 'left' },
        {
          to: '/community',
          label: 'Community',
          position: 'left'
        },
        {
          href: 'https://github.com/errata-ai',
          position: 'right',
          className: 'header-github-link',
          'aria-label': 'GitHub repository'
        },
        {
          href: customFields.slackUrl,
          position: 'right',
          className: 'header-slack-link',
          'aria-label': 'Slack community'
        }
      ]
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Docs',
          items: [
            {
              label: 'Vale CLI',
              to: '/vale/about'
            },
            {
              label: 'Vale Server',
              to: '/vale-server/install/'
            },
            {
              label: 'Vale Studio',
              to: 'https://vale-studio.errata.ai/'
            }
          ]
        },
        {
          title: 'Connect',
          items: [
            {
              label: 'GitHub',
              href: customFields.githubOrg
            },
            {
              label: 'Slack',
              href: customFields.slackUrl
            }
          ]
        },
        {
          title: 'More',
          items: [
            {
              label: 'Sponsor',
              to: 'https://opencollective.com/vale'
            }
          ]
        }
      ],
      copyright: `Copyright © ${new Date().getFullYear()} errata.ai`
    }
  },
  presets: [
    [
      '@docusaurus/preset-classic',
      {
        docs: {
          id: 'docs',
          path: './docs',
          routeBasePath: '/',
          sidebarPath: require.resolve('./sidebars.js'),
          // Please change this to your repo.
          editUrl:
            'https://github.com/errata-ai/vale-server/edit/master/website/'
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          editUrl:
            'https://github.com/errata-ai/vale-server/edit/master/website/blog/'
        },
        theme: {
          customCss: require.resolve('./src/css/_global.css')
        }
      }
    ]
  ]
}

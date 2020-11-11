module.exports = {
  title: "errata.ai",
  tagline: "Innovative tools for collaborative writing",
  url: "https://nervous-almeida-4ca728.netlify.app",
  baseUrl: "/",
  favicon: "img/favicon.ico",
  organizationName: "errata-ai",
  projectName: "vale-server",

  plugins: [],
  scripts: [],
  stylesheets: [],

  themeConfig: {
    announcementBar: {
      id: "supportus",
      backgroundColor: "#e1e4e8",
      isCloseable: false,
      content:
        '⭐️ Check out the new <a target="_blank" rel="noopener noreferrer" href="https://vale-studio.errata.ai">online rule editor</a> for Vale and Vale Server! ⭐️',
    },
    prism: {
      theme: require("prism-react-renderer/themes/github"),
      darkTheme: require("./src/plugins/prism_themes/github"),
      additionalLanguages: ["ini"],
    },
    algolia: {
      apiKey: "12bdf8cd642ab554d424ff89eed88ce9",
      indexName: "errata_ai",
    },
    navbar: {
      hideOnScroll: false,
      title: "errata.ai | Docs",
      logo: {
        alt: "My Site Logo",
        src: "img/logo-alt.svg",
      },
      items: [
        {
          label: "Software",
          position: "left", // or 'right'
          items: [
            {
              label: "Vale",
              to: "/vale/about/",
            },
            {
              label: "Vale Server",
              to: "/vale-server/install/",
            },
            {
              label: "Static School",
              to: "https://staticschool.com/",
            },
          ],
        },
        { to: "blog", label: "Blog", position: "left" },

        // {href: '/api/index.html', label: 'API', position: 'left'},

        {
          href: "https://github.com/errata-ai",
          position: "right",
          className: "header-github-link",
          "aria-label": "GitHub repository",
        },
      ],
    },
    footer: {
      links: [
        {
          title: "About",
          items: [
            {
              label: "Home",
              to: "https://errata.ai",
            },
            {
              label: "Blog",
              to: "Blog",
            },
          ],
        },
        {
          title: "Community",
          items: [
            {
              label: "GitHub",
              href: "https://github.com/errata-ai",
            },
            {
              label: "Slack",
              href: "https://writethedocs.slack.com/archives/CBWQQ5E57",
            },
            {
              label: "Open Collective",
              href: "https://opencollective.com/vale",
            },
          ],
        },
        {
          title: "More",
          items: [
            {
              label: "Become a Sponsor",
              to: "https://opencollective.com/vale",
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} errata.ai`,
    },
  },
  presets: [
    [
      "@docusaurus/preset-classic",
      {
        docs: {
          routeBasePath: "/",
          // It is recommended to set document id as docs home page (`docs/` path).
          // homePageId: 'about',
          sidebarPath: require.resolve("./sidebars.js"),
          // Please change this to your repo.
          editUrl:
            "https://github.com/errata-ai/vale-server/edit/master/website/",
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          editUrl:
            "https://github.com/errata-ai/vale-server/edit/master/website/blog/",
        },
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      },
    ],
  ],
};

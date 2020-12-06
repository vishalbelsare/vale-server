var theme = {
  plain: {
    color: "#ffffff",
    backgroundColor: "#193549",
  },
  styles: [
    {
      types: ["comment", "punctuation"],
      style: {
        color: "rgb(0, 136, 255)",
        fontStyle: "italic",
      },
    },
    {
      types: ["constant"],
      style: {
        color: "rgb(255, 98, 140)",
      },
    },
    {
      types: ["string"],
      style: {
        color: "rgb(165, 255, 144)",
      },
    },
    {
      types: ["builtin"],
      style: {
        color: "rgb(255, 157, 0)",
      },
    },
    {
      types: ["variable"],
      style: {
        color: "rgb(225, 239, 255)",
      },
    },
    {
      types: ["attr-name"],
      style: {
        color: "rgb(255, 180, 84)",
      },
    },
    {
      types: ["tag"],
      style: {
        color: "rgb(158, 255, 255)",
      },
    },
    {
      types: ["keyword"],
      style: {
        color: "rgb(255, 157, 0)",
        fontStyle: "italic",
      },
    },
  ],
};

module.exports = theme;

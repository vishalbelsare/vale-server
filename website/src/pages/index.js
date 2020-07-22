import React from 'react';
import clsx from 'clsx';

import Layout from '@theme/Layout';

import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import useBaseUrl from '@docusaurus/useBaseUrl';

import Products from '@site/src/components/Products';

import styles from './styles.module.css';

const features = [
  {
    title: <>Vale</>,
    imageUrl: 'img/terminal.svg',
    description: (
      <>
        A syntax-aware, command-line linter for prose built with speed and extensibility in mind.
      </>
    ),
  },
  {
    title: <>Vale Server</>,
    imageUrl: 'img/html.svg',
    link: '/about',
    description: (
      <>
        A cross-platform desktop application that brings your editorial style guide to life.
      </>
    ),
  },
  {
    title: <>Static School</>,
    imageUrl: 'img/code.svg',
    description: (
      <>
        Discover, explore, and compare static site generators and other Jamstack-related tools.
      </>
    ),
  },
];

function Feature({imageUrl, title, description, link}) {
  const imgUrl = useBaseUrl(imageUrl);
  return (
    <div className={clsx('col col--4', styles.feature)}>
      {imgUrl && (
        <div className="text--center">
          <a href={link}>
            <img className={styles.featureImage} src={imgUrl} alt={title} />
          </a>
        </div>
      )}
      <h3><a href={link}>{title}</a></h3>
      <p>{description}</p>
    </div>
  );
}

function Home() {
  const context = useDocusaurusContext();
  const {siteConfig = {}} = context;
  return (
    <Layout
      title={`${siteConfig.title}`}
      description="Description will go into a meta tag in <head />">
      <header className={clsx('hero', styles.heroBanner)}>
        <div className="container">
          <h2 className="hero__title">Welcome to the <i>errata.ai</i> docs!</h2>
          <p className="hero__subtitle">Select a product below to get started.</p>
        </div>
      </header>
      <main>
        <Products />
        {/*
        {features && features.length > 0 && (
          <section className={styles.features}>
            <div className="container">
              <div className="row">
                {features.map((props, idx) => (
                  <Feature key={idx} {...props} />
                ))}
              </div>
            </div>
          </section>
                )}*/}
      </main>
    </Layout>
  );
}

export default Home;

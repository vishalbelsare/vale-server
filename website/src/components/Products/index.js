import React from 'react';
import clsx from 'clsx';

import styles from './styles.module.css';

const data = [
  {
    title: <>Vale</>,
    link: '/vale/about',
    description: (
      <>A syntax-aware, command-line linter for prose built with speed and extensibility in mind.</>
    ),
  },
  {
    title: <>Vale Server</>,
    link: '/vale-server/install',
    description: <>A cross-platform desktop application that brings your editorial style guide to life.</>,
  },
  {
    title: <>Static School</>,
    link: 'https://staticschool.com',
    description: <>Discover, explore, and compare static site generators and related Jamstack tools.</>,
  },
];

function Feature({title, link, description}) {
  return (
    <div className={clsx('col col--4', styles.feature)}>
      <a href={link} className="card">
        <div className="card__header">
          <h3>{title}</h3>
        </div>
        <div className="card__body">
          <p>{description}</p>
        </div>
      </a>
    </div>
  );
}

function Products() {
  return (
    <>
      {data && data.length > 0 && (
        <section id="features" className={styles.features}>
          <div className="container">
            <div className="row">
              {data.map((props, idx) => (
                <Feature key={idx} {...props} />
              ))}
            </div>
          </div>
        </section>
      )}
    </>
  );
}

export default Products;

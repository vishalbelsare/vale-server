import React from 'react';
import clsx from 'clsx';

import styles from './styles.module.css';

const data = [
  {
    title: <>Vale</>,
    description: (
      <>A syntax-aware, command-line linter for prose built with speed and extensibility in mind.</>
    ),
  },
  {
    title: <>Vale Server</>,
    description: <>A cross-platform desktop application that brings your editorial style guide to life.</>,
  },
  {
    title: <>Static School</>,
    description: <>Discover, explore, and compare static site generators and related Jamstack tools.</>,
  },
];

function Feature({title, description}) {
  return (
    <div className={clsx('col col--4', styles.feature)}>
      <a href="/foo" className="card">
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

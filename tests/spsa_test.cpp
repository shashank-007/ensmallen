/**
 * @file spsa_test.cpp
 * @author N Rajiv Vaidyanathan
 * @author Marcus Edel
 *
 * Test file for the SPSA optimizer.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#include <ensmallen.hpp>
#include "catch.hpp"
#include "test_function_tools.hpp"

using namespace arma;
using namespace ens;
using namespace ens::test;

/**
 * Test the SPSA optimizer on the SGDTest function.
 */
TEST_CASE("SPSASimpleSGDTestFunction","[SPSATest]")
{
  SGDTestFunction f;
  SPSA optimizer(0.1, 1, 0.102, 0.16, 0.3, 100000, 0);

  arma::mat coordinates = f.GetInitialPoint();
  coordinates.ones();
  double result = optimizer.Optimize(f, coordinates);

  REQUIRE(result == Approx(-1.0).epsilon(0.0005));
  REQUIRE(coordinates[0] == Approx(0.0).margin(1e-3));
  REQUIRE(coordinates[1] == Approx(0.0).margin(1e-7));
  REQUIRE(coordinates[2] == Approx(0.0).margin(1e-7));
}

/**
 * Test the SPSA optimizer on the Sphere function.
 */
TEST_CASE("SPSASphereFunctionTest", "[SPSATest]")
{
  SphereFunction f(2);
  SPSA optimizer(0.1, 2, 0.102, 0.16, 0.3, 100000, 0);

  arma::mat coordinates = f.GetInitialPoint();
  optimizer.Optimize(f, coordinates);

  REQUIRE(coordinates[0] == Approx(0.0).margin(0.1));
  REQUIRE(coordinates[1] == Approx(0.0).margin(0.1));
}

/**
 * Test the SPSA optimizer on the Matyas function.
 */
TEST_CASE("SPSAMatyasFunctionTest", "[SPSATest]")
{
  MatyasFunction f;
  SPSA optimizer(0.1, 1, 0.102, 0.16, 0.3, 100000, 0);

  arma::mat coordinates = f.GetInitialPoint();
  optimizer.Optimize(f, coordinates);

  // 3% error tolerance.
  REQUIRE((std::trunc(100.0 * coordinates[0]) / 100.0) ==
      Approx(0.0).epsilon(0.003));
  REQUIRE((std::trunc(100.0 * coordinates[1]) / 100.0) ==
      Approx(0.0).epsilon(0.003));
}

/**
 * Run SPSA on logistic regression and make sure the results are acceptable.
 */
TEST_CASE("SPSALogisticRegressionTest", "[SPSATest]")
{
  arma::mat data, testData, shuffledData;
  arma::Row<size_t> responses, testResponses, shuffledResponses;

  LogisticRegressionTestData(data, testData, shuffledData,
      responses, testResponses, shuffledResponses);
  LogisticRegression<> lr(shuffledData, shuffledResponses, 0.5);

  SPSA optimizer(0.1, 1, 0.102, 0.16, 0.3, 100000, 1e-8);
  arma::mat coordinates = lr.GetInitialPoint();
  optimizer.Optimize(lr, coordinates);

  // Ensure that the error is close to zero.
  const double acc = lr.ComputeAccuracy(data, responses, coordinates);
  REQUIRE(acc == Approx(100.0).epsilon(0.003)); // 0.3% error tolerance.

  const double testAcc = lr.ComputeAccuracy(testData, testResponses,
      coordinates);
  REQUIRE(testAcc == Approx(100.0).epsilon(0.006)); // 0.6% error tolerance.
}

#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <chrono> // For timing measurements

// Round half to even (also known as bankers' rounding)
double roundToEven(double value, int decimalPlaces) {
  double factor = std::pow(10, decimalPlaces); // Calculate 10^decimalPlaces
  double scaledValue = value * factor; // Scale up the value
  double fractionalPart = scaledValue - std::floor(scaledValue); // Extract the fractional part

  // Determine whether to round up or down
  if (fractionalPart > 0.5) {
    scaledValue = std::ceil(scaledValue); // Greater than 5, round up
  } else if (fractionalPart < 0.5) {
    scaledValue = std::floor(scaledValue); // Less than 5, round down
  } else {
    // Exactly 5, check the last digit of the retained part
    if (static_cast<int>(scaledValue) % 2 == 0) {
      scaledValue = std::floor(scaledValue); // Even number, round down
    } else {
      scaledValue = std::ceil(scaledValue); // Odd number, round up
    }
  }

  return scaledValue / factor; // Scale back to original range
}

// Standard rounding
double roundNormal(double value, int decimalPlaces) {
  double factor = std::pow(10, decimalPlaces); // Calculate 10^decimalPlaces
  return std::round(value * factor) / factor; // Perform standard rounding
}

// Dynamically determine the number of decimal places and perform rounding
std::pair<int, double> determineDecimalPlacesAndRound(double value, bool useRoundToEven) {
  if (value == 0.0) return {0, 0.0}; // If the value is 0, directly return 0 decimal places and 0.0

  int decimalPlaces = 0; // Initialize decimal places to 0
  double roundedValue = std::round(value); // Initial rounded value as integer part

  // Loop until the error between the rounded value and the original value is less than 1e-7
  while (std::abs(value - roundedValue) > 1e-7) {
    decimalPlaces++; // Increase decimal places
    roundedValue = useRoundToEven ? roundToEven(value, decimalPlaces) : roundNormal(value, decimalPlaces); // Choose rounding method based on parameter
  }

  return {decimalPlaces, roundedValue}; // Return the number of decimal places and the rounded value
}

// Test function to run multiple times and get average time
template<typename Func>
double measureTime(Func func, int iterations = 100000) {
  auto start = std::chrono::high_resolution_clock::now(); // Start timing
  for (int i = 0; i < iterations; ++i) {
    func(); // Run the function multiple times
  }
  auto end = std::chrono::high_resolution_clock::now(); // End timing
  std::chrono::duration<double, std::nano> elapsed = end - start; // Calculate total elapsed time (nanoseconds)
  return elapsed.count() / iterations; // Return average time per run (nanoseconds)
}

int main() {
  std::string num = "27723.15999999997"; // Input string
  double value = std::stod(num); // Convert string to double type

  // Test round to even
  auto timeEven = measureTime([&]() {
    determineDecimalPlacesAndRound(value, true); // Use round to even method
  });

  auto result1 = determineDecimalPlacesAndRound(value, true); // Get rounding result
  int decimalPlaces1 = result1.first; // Number of decimal places to keep
  double roundedValue1 = result1.second; // Rounded value

  // Output round to even results
  std::cout << "Round to Even:" << std::endl;
  std::cout << "Original value: " << std::fixed << std::setprecision(15) << value << std::endl;
  std::cout << "Decimal places to keep: " << decimalPlaces1 << std::endl;
  std::cout << "Rounded value: " << std::fixed << std::setprecision(decimalPlaces1) << roundedValue1 << std::endl;
  std::cout << "Average time per run: " << timeEven << " nanoseconds" << std::endl;
  std::cout << std::endl;

  // Test standard rounding
  auto timeNormal = measureTime([&]() {
    determineDecimalPlacesAndRound(value, false); // Use standard rounding method
  });

  auto result2 = determineDecimalPlacesAndRound(value, false); // Get rounding result
  int decimalPlaces2 = result2.first; // Number of decimal places to keep
  double roundedValue2 = result2.second; // Rounded value

  // Output standard rounding results
  std::cout << "Round Normal:" << std::endl;
  std::cout << "Original value: " << std::fixed << std::setprecision(15) << value << std::endl;
  std::cout << "Decimal places to keep: " << decimalPlaces2 << std::endl;
  std::cout << "Rounded value: " << std::fixed << std::setprecision(decimalPlaces2) << roundedValue2 << std::endl;
  std::cout << "Average time per run: " << timeNormal << " nanoseconds" << std::endl;

  return 0;
}
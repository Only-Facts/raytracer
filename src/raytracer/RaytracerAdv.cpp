/**************************************************************\
Edition:
##  @date 16/05/2026 by @author Tsukini

File Name:
##  @file RaytracerAdv.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#define _Write
#include "utils/utils.hpp"
#include "raytracer/special/Utils.hpp"
#include "raytracer/Raytracer.hpp"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>
#include <cmath>

static hot nodiscard std::string formatNumber(double n)
{
    static const char* const suffixes[] = {"", "K", "M", "B", "T"};
    std::size_t i = 0;

    // Get the suffixes
    for (i = 0; n >= 1000.0 && i < 4; ++i)
        n /= 1000.0;

    // Get the precision
    std::size_t precision =
    (n < 2.0)    ? 3 :
    (n < 10.0)   ? 2 :
    (n < 100.0)  ? 1 : 0;

    // Format the string
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << n << suffixes[i];
    return oss.str();
}

static hot nodiscard std::string formatTime(long long seconds)
{
    long long h = seconds / 3600;
    long long m = (seconds % 3600) / 60;
    long long s = seconds % 60;
    return std::to_string(h) + "h " + std::to_string(m) + "min " + std::to_string(s) + "s";
}

hot void raytracer::Raytracer::adv(bool forced, bool increment)
{
    static const char *const steps[] = {"Scene Initialisation", "Light Rays", "Camera Rays"};

    // Check if the advencement need to be dump
    if (!this->_settings.adv) return;

    // Update only on each x advencement
    if (increment) ++this->_adv;
    if (!this->_settings.debug && !forced && this->_adv % 1000 == 0) return;

    // Try to lock
    std::unique_lock<std::mutex> lock(this->_advLock, std::try_to_lock);
    if (!lock.owns_lock()) return;

    // Computing
    float percent = (this->_advMax == 0) ? 1.0f : (static_cast<float>(this->_adv) / static_cast<float>(this->_advMax));
    std::size_t done = std::ceil(percent * ADV_SIZE);

    // Compute timer
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    long long elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - this->_timer).count();
    std::string elapsedStr = formatTime(elapsed);

    // Build bar
    std::string bar(ADV_SIZE, '-');
    for (std::size_t i = 0; i < done && i < ADV_SIZE; ++i)
        bar[i] = '/';
    if (!this->isGui()) {
        std::size_t start = (ADV_SIZE > elapsedStr.size()) ? (ADV_SIZE - elapsedStr.size()) / 2 : 0;
        for (std::size_t i = 0; i < elapsedStr.size() && start + i < ADV_SIZE; ++i)
            bar[start + i] = elapsedStr[i];
    }
    std::ostringstream out;
    for (std::size_t i = 0; i < ADV_SIZE; ++i) {
        switch (bar[i]) {
            case '/':
                out << utils::write::color_rgb(0, 255, 0) << "/";
                break;
            case '-':
                out << utils::write::color_rgb(255, 0, 0) << "-";
                break;
            default:
                out << utils::write::reset() << utils::write::strong() << bar[i];
                break;
        }
    }

    // Display bar
    std::cout << "\r" << utils::write::reset();
    std::cout << utils::write::strong() << "[";
    std::cout << out.str();
    std::cout << utils::write::reset() << utils::write::strong() << "] ";

    // Counters
    if (this->_settings.debug) std::cout << this->_adv << "/" << this->_advMax;
    else std::cout << formatNumber(this->_adv) << "/" << formatNumber(this->_advMax);

    // Step
    std::cout << " Step (" << this->_step + 1 << "/" << MAX_STEP << "): " << steps[this->_step % MAX_STEP];
    std::cout << utils::write::reset() << std::flush;

    /*
    // Advencement display
    std::cout << utils::write::line() << "\r" << utils::write::reset();
    std::cout << utils::write::strong() << "[";
    std::cout << utils::write::color_rgb(0, 255, 0);
    for (std::size_t i = 0; i < done; ++i) std::cout << "/";
    std::cout << utils::write::color_rgb(255, 0, 0);
    for (std::size_t i = done; i < ADV_SIZE; ++i) std::cout << "-";
    std::cout << utils::write::reset() << utils::write::strong() << "]";
    if (this->_settings.debug) std::cout << " - " << this->_adv << "/" << this->_advMax;
    else std::cout << " - " << formatNumber(this->_adv) << "/" << formatNumber(this->_advMax);
    std::cout << " Step (" << this->_step + 1 << "/" << MAX_STEP << "): " << steps[this->_step % MAX_STEP];
    std::cout << utils::write::reset() << std::flush;
    */
}

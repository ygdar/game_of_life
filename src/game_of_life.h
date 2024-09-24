//
// Created by Administrator on 9/5/2024.
//

#ifndef GRAPHICA_GAME_OF_LIFE_H
#define GRAPHICA_GAME_OF_LIFE_H

#include <memory>
#include <random>
#include <algorithm>
#include <execution>
#include <ranges>
#include <Eigen/Eigen>

namespace gol {
    class GameOfLife{
    public:
        GameOfLife(int xSize, int ySize){
            field_ = std::make_unique<GameOfLifeField_>(xSize, ySize, 1);
        }

        void setRandomSeed()
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);

            for (int ix = 0; ix < field_->getWidth(); ix++)
            {
                for (int jx = 0; jx < field_->getHeight(); jx++)
                {
                    field_->preset(ix, jx, dist2(rng) == 1);
                }
            }

            field_->update();
        }

        void update();
        bool getCellStatus(int ix, int y);
    private:
        class GameOfLifeField_ {
        public:
            GameOfLifeField_(int width, int height, int offset);

            [[nodiscard]]int getWidth() const;
            [[nodiscard]]int getHeight() const;

            void preset(int x, int y, bool value);
            void update();

            int convolute(const Eigen::MatrixXi& kernel_, int x, int y);
            bool operator()(int x, int y) const;

        private:
            int width_;
            int height_;
            int offset_;
            Eigen::MatrixXi field_;
            Eigen::MatrixXi buffer_;
        };

        std::unique_ptr<GameOfLifeField_> field_;
        Eigen::MatrixXi kernel = Eigen::MatrixXi({
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1}
        });
    };
}



#endif //GRAPHICA_GAME_OF_LIFE_H

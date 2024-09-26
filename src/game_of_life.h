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

#include "game_field.h"
#include "game_rule.h"

namespace gol {
    class GameOfLife{
    public:
        GameOfLife(int xSize, int ySize){
            field_ = std::make_unique<gol::GameField>(xSize, ySize, 3);
            rule_ = std::make_unique<gol::GameRule>();
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

        void prepare()
        {
            for (int ix = 0; ix < field_->getWidth(); ix++)
            {
                for (int jx = 0; jx < field_->getHeight(); jx++)
                {
                    int count = field_->apply(rule_->getKernel(), ix, jx);
                    bool cellStatus = rule_->getCellStatus(field_->at(ix, jx), count);

                    field_->preset(ix, jx, cellStatus);
                }
            }
        }

        void update()
        {
            field_->update();
        }

        bool getCellStatus(int x, int y)
        {
            return field_->at(x, y);
        }

    private:
        std::unique_ptr<gol::GameField> field_;
        std::unique_ptr<gol::GameRule> rule_;
    };
}



#endif //GRAPHICA_GAME_OF_LIFE_H

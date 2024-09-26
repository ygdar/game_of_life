//
// Created by Administrator on 9/25/2024.
//

#ifndef GRAPHICA_GAME_RULE_H
#define GRAPHICA_GAME_RULE_H

#include <memory>
#include <unordered_set>

#include <Eigen/Core>

#include "game_field.h"

namespace gol {
    class GameRule
    {
    public:
        GameRule()
        {
            born_ = { 3 };
            survive_ = { 2, 3 };
            kernel_ = std::make_shared<Eigen::MatrixXi>(Eigen::MatrixXi({
                {1, 1, 1},
                {1, 0, 1},
                {1, 1, 1}
            }));
        }

        std::shared_ptr<Eigen::MatrixXi> getKernel() const
        {
            return kernel_;
        }

        bool getCellStatus(bool alive, int value)
        {
            if (!alive && born_.contains(value))
            {
                return true;
            }

            if (alive && survive_.contains(value))
            {
                return true;
            }

            return false;
        }

    private:
        std::unordered_set<int> born_;
        std::unordered_set<int> survive_;
        std::shared_ptr<Eigen::MatrixXi> kernel_;
    };
}
#endif //GRAPHICA_GAME_RULE_H

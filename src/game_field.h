//
// Created by Administrator on 9/25/2024.
//

#ifndef GRAPHICA_GAME_FIELD_H
#define GRAPHICA_GAME_FIELD_H

#include <Eigen/Core>

namespace gol {
    class GameField {
    public:
        GameField(int width, int height, int offset) : width_(width), height_(height), offset_(offset)
        {
            field_ = Eigen::MatrixXi(width_ + 2 * offset_, height_ + 2 * offset_);
            buffer_ = Eigen::MatrixXi(width_ + 2 * offset_, height_ + 2 * offset_);

            field_.setZero();
            buffer_.setZero();
        }

        [[nodiscard]]
        int getWidth() const
        {
            return width_;
        }

        [[nodiscard]]
        int getHeight() const
        {
            return height_;
        }

        void preset(int x, int y, bool alive)
        {
            assert(0 <= x && x < getWidth());
            assert(0 <= y && y < getHeight());

            int xShifted = x + offset_, yShifted = y + offset_;

            buffer_(xShifted, yShifted) = alive;
        }

        void update()
        {
            field_ = buffer_;
        }

        bool at(int x, int y)
        {
            assert(0 <= x && x < getWidth());
            assert(0 <= y && y < getHeight());

            int xShifted = x + offset_, yShifted = y + offset_;

            return field_(xShifted, yShifted);
        }

        int apply(const std::shared_ptr<Eigen::MatrixXi>& kernel, int x, int y)
        {
            assert(kernel->rows() <= 2 * offset_ + 1);
            assert(kernel->cols() <= 2 * offset_ + 1);

            int kernelXOffset = (kernel->cols() - 1) / 2;
            int kernelYOffset = (kernel->rows() - 1) / 2;

            auto block =
                field_.block<int, int>(x + (offset_ - kernelXOffset), y + (offset_ - kernelYOffset), kernel->rows(), kernel->cols());

            return (kernel->transpose() * block).trace();
        }

    private:
        int width_{};
        int height_{};
        int offset_{};
        Eigen::MatrixXi field_;
        Eigen::MatrixXi buffer_;
    };

}

#endif //GRAPHICA_GAME_FIELD_H

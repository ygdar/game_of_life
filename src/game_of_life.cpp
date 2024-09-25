//
// Created by Administrator on 9/5/2024.
//

#include "game_of_life.h"

namespace gol{
    void GameOfLife::update(){
        field_->update();
    }

    bool GameOfLife::getCellStatus(int x, int y){
        return (*field_)(x, y);
    }

    void GameOfLife::prepare(){

        for (int ix = 0; ix < field_->getWidth(); ix++)
        {
            for (int jx = 0; jx < field_->getHeight(); jx++){
                auto sum = field_->convolute(kernel, ix, jx);
                if (sum < 2)
                {
                    field_->preset(ix, jx, false);
                } else if ((*field_)(ix, jx) && (2 <= sum && sum <= 3)){
                    // continue
                } else if ((*field_)(ix, jx) && (3 < sum)){
                    field_->preset(ix, jx, false);
                } else if (!(*field_)(ix, jx) && (sum == 3)){
                    field_->preset(ix, jx, true);
                }
            }
        }
    }

    int GameOfLife::GameOfLifeField_::convolute(const Eigen::MatrixXi &kernel_, int x, int y){
        assert(kernel_.rows() == 2 * offset_ + 1);
        assert(kernel_.cols() == 2 * offset_ + 1);

        int kernelSide = 2 * offset_ + 1;

        auto block =
            field_.block<int, int>(x, y, kernelSide, kernelSide);

        return (kernel_.transpose() * block).trace();
    }

    bool GameOfLife::GameOfLifeField_::operator()(int x, int y) const{
        assert(0 <= x && x < getWidth());
        assert(0 <= y && y < getHeight());

        int xShifted = x + offset_, yShifted = y + offset_;

        return field_(xShifted, yShifted) == 1;
    }

    GameOfLife::GameOfLifeField_::GameOfLifeField_(int width, int height, int offset)
        : width_(width), height_(height), offset_(offset){
        field_ = Eigen::MatrixXi(width_ + 2 * offset_, height_ + 2 * offset_);
        buffer_ = Eigen::MatrixXi(width_ + 2 * offset_, height_ + 2 * offset_);

        field_.setZero();
        buffer_.setZero();
    }

    int GameOfLife::GameOfLifeField_::getWidth() const{
        return width_;
    }

    int GameOfLife::GameOfLifeField_::getHeight() const{
        return height_;
    }

    void GameOfLife::GameOfLifeField_::preset(int x, int y, bool value){
        assert(0 <= x && x < getWidth());
        assert(0 <= y && y < getHeight());

        int xShifted = x + offset_, yShifted = y + offset_;

        buffer_(xShifted, yShifted) = value;
    }

    void GameOfLife::GameOfLifeField_::update(){
        field_ = buffer_;
    }
}
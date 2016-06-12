//
//  SelectionCell.m
//  ComboBox
//
//  Created by Eric Che on 7/17/13.
//  Copyright (c) 2013 Eric Che. All rights reserved.
//

#import "SelectionCell.h"

@implementation SelectionCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.backgroundColor = COLOR_TABLEBLOCK_CELL_BG;
        CGRect frame = self.frame;
        frame.origin.y += 10;
        UIImage *img = [UIImage createImageWithColor:COLOR_GOLD_THEME];
        UIImageView *imgView = [[UIImageView alloc] initWithFrame:frame];
        imgView.image = img;
        self.selectedBackgroundView = imgView;
        frame = self.frame;
        frame.origin.x += 20;
        _lb = [[UILabel alloc] initWithFrame:frame];
        _lb.textAlignment = NSTextAlignmentLeft;
        _lb.textColor = COLOR_TABLEBLOCK_CELL_TEXT;
        [self.contentView addSubview:_lb];
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    _lb.highlightedTextColor = COLOR_TABLEBLOCK_CELL_SELECTED_TEXT;
    // Configure the view for the selected state
}

@end

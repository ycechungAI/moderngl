# WebGL Water
# https://madebyevan.com/webgl-water/
# Copyright 2011 Evan Wallace
# Released under the MIT license

from typing import Self
import glm
import math
from light_gl import Matrices


class HitTest:
    def __init__(self, t: float, hit: glm.vec3, normal: glm.vec3) -> None:
        self.t = t
        self.hit = hit
        self.normal = normal

    def merge_with(self, other: Self):
        if other.t > 0 and other.t < self.t:
            self.t = other.t
            self.hit = other.hit
            self.normal = other.normal

    def __repr__(self) -> str:
        return f"t: {self.t} hit: {self.hit} norm: {self.normal}"


class RayTracer:
    def __init__(self, viewport: tuple[float], matrices: Matrices) -> None:
        model_view_matrix = matrices.model_view
        self.eye = matrices.eye
        self.viewport = viewport

        min_x = viewport[0]
        max_x = min_x + viewport[2]
        min_y = viewport[1]
        max_y = min_y + viewport[3]

        self.ray00 = glm.unProject(
            (min_x, min_y, 1), model_view_matrix, matrices.projection, viewport) - self.eye
        self.ray10 = glm.unProject(
            (max_x, min_y, 1), model_view_matrix, matrices.projection, viewport) - self.eye
        self.ray01 = glm.unProject(
            (min_x, max_y, 1), model_view_matrix, matrices.projection, viewport) - self.eye
        self.ray11 = glm.unProject(
            (max_x, max_y, 1), model_view_matrix, matrices.projection, viewport) - self.eye

    def get_ray_for_pixel(self, x, y) -> glm.vec3:
        x = (x - self.viewport[0]) / self.viewport[2]
        y = 1.0 - (y - self.viewport[1]) / self.viewport[3]
        ray0 = glm.lerp(self.ray00, self.ray10, x)
        ray1 = glm.lerp(self.ray01, self.ray11, x)
        return glm.normalize(glm.lerp(ray0, ray1, y))

    def hit_test_sphere(origin: glm.vec3, ray: glm.vec3, center: glm.vec3, radius: float) -> HitTest:
        offset = origin - center
        a = glm.dot(ray, ray)
        b = 2 * glm.dot(ray, offset)
        c = glm.dot(offset, offset) - radius * radius
        discriminant = b * b - 4 * a * c
        if discriminant > 0:
            t = (-b - math.sqrt(discriminant)) / (2*a)
            hit = origin + ray * t
            return HitTest(t, hit, (hit - center) / radius)
        return None

    def hit_test_triangle(origin: glm.vec3, ray: glm.vec3, vert: tuple[glm.vec3]):
        ab = vert[1] - vert[0]
        ac = vert[2] - vert[0]
        normal = glm.normalize(glm.cross(ab, ac))
        t = glm.dot(normal, vert[0] - origin) / glm.dot(normal, ray)
        if t > 0:
            hit = origin + (ray * t)
            to_hit = hit - vert[0]
            # uv = glm.inverse((glm.mat3(ac, ab, normal))) * to_hit
            dot00 = glm.dot(ac, ac)
            dot01 = glm.dot(ab, ac)
            dot11 = glm.dot(ab, ab)
            dot02 = glm.dot(ac, to_hit)
            dot12 = glm.dot(ab, to_hit)
            divide = dot00 * dot11 - dot01 * dot01
            u = (dot11 * dot02 - dot01 * dot12) / divide
            v = (dot00 * dot12 - dot01 * dot02) / divide
            print("uv", u, v)
            if u >= 0 and v >= 0 and (u + v) <= 1:
                return HitTest(t, hit, normal)
        return None
